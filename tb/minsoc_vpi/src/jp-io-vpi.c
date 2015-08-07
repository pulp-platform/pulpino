/* jp-io-vpi.c -- JTAG communications vpi plugin
   Copyright (C) 2004 György Jeney, nog@sdf.lonestar.org
   Modifications copyright (C) 2008 Nathan Yawn, nathan.yawn@opencores.org
   
   This file is part of OpenRISC 1000 Architectural Simulator.
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* This plugs into an rtl simulator via vpi */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif

#include <vpi_user.h>

/* The vpi<->jp connection is `mastered' by jp1.  Therefore we just sit doing
 * `nothing', waiting for jp1 to request or send us some data */
static uint8_t vpi_out; /* data that the sim gives to us */

#ifdef WIN32
SOCKET jp_comm_m;
SOCKET jp_comm;
char msgbuf[64];
char * get_ws_error(void);
#define GET_ERR_MSG get_ws_error()
#ifndef EAGAIN
#define EAGAIN WSAEWOULDBLOCK
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#else
static int jp_comm_m; /* The listening socket */
static int jp_comm; /* The socket for communitateing with jp1 */
#define GET_ERR_MSG strerror(errno)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif


static int jp_got_con; /* Have we got a connection ? */

static int count_comp; /* Has the predetermined cycle-count been reached ? */
static int jp_waiting; /* Is jp-waiting for count_comp ? */

int jp_check_con();

/*---------------------------------------------[ VPI interface to the sim ]---*/
/* Sends a byte from the sim */
int vpi_jp_out(char *xx)
{
  vpiHandle sys = vpi_handle(vpiSysTfCall, 0);
  vpiHandle argv = vpi_iterate(vpiArgument, sys);
  s_vpi_value value;

  if(!argv) {
    vpi_printf("$jp_out: missing destination argument\n");
    vpi_free_object(argv);
    return 0;
  }
  vpiHandle dat_to = vpi_scan(argv);
  if(vpi_get(vpiType, dat_to) != vpiNet) {
    vpi_printf("$jp_out: Must have a net as argument!!\n");
    vpi_free_object(argv);
    return 0;
  }

  value.format = vpiVectorVal;
  vpi_get_value(dat_to, &value);

  if((value.value.vector->bval & 1)) {
    vpi_free_object(argv);
    return 0;
  }
  vpi_out = value.value.vector->aval & 1;

  vpi_free_object(argv);

  return 0;
}

/* Sends a byte to the sim */
int vpi_jp_in(char *xx)
{
  int ret;
  uint8_t dat;
  s_vpi_vecval vec;

  s_vpi_value value;
  vpiHandle sys = vpi_handle(vpiSysTfCall, 0);
  vpiHandle argv;
  vpiHandle dat_to;

  if(!jp_got_con) {
    if(!jp_check_con())
      return 0;
  }

  ret = recv(jp_comm, &dat, 1, 0); 
  if(!ret)
    return 0;
  if(ret == SOCKET_ERROR) {
#ifdef WIN32
    if(WSAGetLastError() == WSAEWOULDBLOCK)
#else
    if(errno == EAGAIN)
#endif
    {
      return 0;
    }
    else {
      vpi_printf("Socket recv error: %s\n", GET_ERR_MSG);
      return 0;
    }
  }


  if(dat & 0x80) {
    switch(dat & 0x7f) {
    case 0:
      /* jp1 wants the TDO */
      send(jp_comm, &vpi_out, 1, 0);
      return 0;
    case 1:
      /* jp wants a time-out */
      if(count_comp) {
        dat = 0xFF;                   /* A value of 0xFF is expected, but not required */
        send(jp_comm, &dat, 1, 0);
      }
      else {
        jp_waiting = 1;
      }
      return 0;
    }
  }

  argv = vpi_iterate(vpiArgument, sys);

  /* We got the data, acknowledge it and send it on to the sim */
  if(!argv) {
    vpi_printf("$jp_in: missing destination argument\n");
    vpi_free_object(argv);
    return 0;
  }
  dat_to = vpi_scan(argv);
  if(vpi_get(vpiType, dat_to) != vpiReg) {
    vpi_printf("$jp_in: Must have a register (vpiReg) as argument (type is %d)!!\n", vpi_get(vpiType, dat_to));
    vpi_free_object(argv);
    return 0;
  }

  value.format = vpiVectorVal;

  vec.aval = (dat & 0xf) | 0x10;
  vec.bval = 0;
  value.value.vector = &vec;
  vpi_put_value(dat_to, &value, 0, vpiNoDelay);

  vpi_free_object(argv);

  dat |= 0x10;
  ret = send(jp_comm, &dat, 1, 0);

  count_comp = 0;

  return 0;
}

/* tells us that we reached a predetermined cycle count */
int jp_wait_time(char *xx)
{
  uint8_t dat = 0xFF;
  if(jp_waiting) {
    send(jp_comm, &dat, 1, 0);
    jp_waiting = 0;
  }

  count_comp = 1;
  return 0;
}

/*---------------------------------------------------[ VPI<->jp functions ]---*/
int init_sock(char *xx)
{
  struct sockaddr_in addr;
  int ret;
  vpiHandle sys = vpi_handle(vpiSysTfCall, 0);
  vpiHandle argv = vpi_iterate(vpiArgument, sys);
  s_vpi_value value;

  if(!argv) {
    vpi_printf("$jp_init: missing port argument\n");
    return 0;
  }
  vpiHandle sock = vpi_scan(argv);
/* orig
  if(vpi_get(vpiConstType, sock) != vpiStringConst) {
    vpi_printf("$jp_init: Must have a string as argument!!\n");
    vpi_free_object(argv);
    return 0;
  }
*/

#ifdef WIN32
  WSADATA wsaData;
  ret = WSAStartup(MAKEWORD(2,2), &wsaData);  // must be called before all socket operations
  if(ret != 0) {
    vpi_printf("$jp_init: Winsock startup failed.");
    return 0;
  }
#endif

  value.format = vpiStringVal;
  vpi_get_value(sock, &value);

  addr.sin_family = AF_INET;
  addr.sin_port = htons((uint16_t)atoi(value.value.str));
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  jp_comm_m = socket(PF_INET, SOCK_STREAM, 0);
#ifdef WIN32
  if(jp_comm_m == INVALID_SOCKET)
#else
  if(jp_comm_m < 0)
#endif
   {
     fprintf(stderr, "Unable to create comm socket: %s\n", GET_ERR_MSG);
    return 0;
  }

  int yes = 1;
  if(setsockopt(jp_comm_m, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    fprintf(stderr, "Unable to setsockopt on the socket: %s\n", GET_ERR_MSG);
    return 0;
  }

  if(bind(jp_comm_m, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
    fprintf(stderr, "Unable to bind the socket: %s\n", GET_ERR_MSG);
    return 0;
  }

  if(listen(jp_comm_m, 1) == SOCKET_ERROR) {
    fprintf(stderr, "Unable to listen: %s\n", GET_ERR_MSG);
    return 0;
  }

#ifdef WIN32
  u_long arg = 1;
  ioctlsocket(jp_comm_m, FIONBIO, &arg);
#else
  ret = fcntl(jp_comm_m, F_GETFL);
  ret |= O_NONBLOCK;
  fcntl(jp_comm_m, F_SETFL, ret);
#endif

  jp_got_con = 0;
  jp_waiting = 0;
  return 0;
}

/* Checks to see if we got a connection */
int jp_check_con()
{
  int ret;

  if((jp_comm = accept(jp_comm_m, NULL, NULL)) == SOCKET_ERROR) {
#ifdef WIN32
    if(WSAGetLastError() == WSAEWOULDBLOCK)
#else
    if(errno == EAGAIN)
#endif
      return 0;
    fprintf(stderr, "Unable to accept connection: %s\n", GET_ERR_MSG);
    return 0;
  }


  // Set the comm socket to non-blocking.
  // Close the server socket, so that the port can be taken again
  // if the simulator is reset.
#ifdef WIN32
  u_long arg = 1;
  ioctlsocket(jp_comm, FIONBIO, &arg);
  closesocket(jp_comm_m);
#else
  ret = fcntl(jp_comm, F_GETFL);
  ret |= O_NONBLOCK;
  fcntl(jp_comm, F_SETFL, ret);
  close(jp_comm_m);
#endif

  vpi_printf("JTAG communication connected!\n");
  jp_got_con = 1;
  return 1;
}

/*------------------------------------------------------------[ VPI stuff ]---*/
static void jtag_register()
{
  s_vpi_systf_data tf_data;

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$jp_in";
  tf_data.calltf    = vpi_jp_in;
  tf_data.compiletf = 0;
  tf_data.sizetf    = 0;
  vpi_register_systf(&tf_data);

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$jp_out";
  tf_data.calltf    = vpi_jp_out;
  tf_data.compiletf = 0;
  tf_data.sizetf    = 0;
  vpi_register_systf(&tf_data);

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$jp_init";
  tf_data.calltf    = init_sock;
  tf_data.compiletf = 0;
  tf_data.sizetf    = 0;
  vpi_register_systf(&tf_data);

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$jp_wait_time";
  tf_data.calltf    = jp_wait_time;
  tf_data.compiletf = 0;
  tf_data.sizetf    = 0;
  vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
      jtag_register,
      0
};


#ifdef WIN32
char *get_ws_error(void) 
{
  snprintf(msgbuf, 64, "%d", WSAGetLastError()); 
  return msgbuf;
}
#endif

