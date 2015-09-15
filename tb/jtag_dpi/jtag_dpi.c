
#include "svdpi.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "dpiheader.h"


extern svLogic rtl_get_tdo();

int jp_check_con();

uint8_t jp_waiting;
uint8_t count_comp;
uint8_t jp_got_con;

static int jp_comm_m; // The listening socket
static int jp_comm;   // The socket for communicating with jp

int socket_port;


void socket_open() {
  struct sockaddr_in addr;
  int ret;

  count_comp = 0;
  jp_waiting = 0;
  jp_got_con = 0;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(socket_port);
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  jp_comm_m = socket(PF_INET, SOCK_STREAM, 0);
  if(jp_comm_m < 0)
  {
    fprintf(stderr, "Unable to create comm socket: %s\n", strerror(errno));
    return;
  }

  int yes = 1;
  if(setsockopt(jp_comm_m, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    fprintf(stderr, "Unable to setsockopt on the socket: %s\n", strerror(errno));
    return;
  }

  if(bind(jp_comm_m, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    fprintf(stderr, "Unable to bind the socket: %s\n", strerror(errno));
    return;
  }

  if(listen(jp_comm_m, 1) == -1) {
    fprintf(stderr, "Unable to listen: %s\n", strerror(errno));
    return;
  }

  ret = fcntl(jp_comm_m, F_GETFL);
  ret |= O_NONBLOCK;
  fcntl(jp_comm_m, F_SETFL, ret);

  fprintf(stderr, "Listening on port %d\n", socket_port);
}

int jtag_recv(svLogic* tck, svLogic* trstn, svLogic* tdi, svLogic* tms) {
  uint8_t tdo;
  uint8_t dat;
  int ret;

  if(!jp_got_con) {
    if(!jp_check_con()) {
      return 0;
    }
  }

  ret = recv(jp_comm, &dat, 1, 0);

  // check connection abort
  if((ret == -1 && errno != EWOULDBLOCK) || (ret == 0)) {
    printf("JTAG Connection closed\n");

    close(jp_comm);
    socket_open();
    return 0;
  }

  // no available data
  if(ret == -1 && errno == EWOULDBLOCK)
    return 0;

  if(dat & 0x80) {
    switch(dat & 0x7f) {
      case 0:
        // jp wants the TDO
        tdo = rtl_get_tdo();

        // convert from logic to 0/1
        if (tdo == sv_1)
          tdo = 1;
        else
          tdo = 0;

        send(jp_comm, &tdo, 1, 0);
        return 0;
      case 1:
        // jp wants a time-out
        if(count_comp) {
          dat = 0xFF; // A value of 0xFF is expected, but not required
          send(jp_comm, &dat, 1, 0);
        }
        else {
          jp_waiting = 1;
        }
        return 0;
    }
  }

  *tck   = (dat & 0x1) >> 0;
  *trstn = (dat & 0x2) >> 1;
  *tdi   = (dat & 0x4) >> 2;
  *tms   = (dat & 0x8) >> 3;

  dat |= 0x10;
  ret = send(jp_comm, &dat, 1, 0);

  return 1;
}

void jtag_timeout() {
  uint8_t dat = 0xFF;
  if(jp_waiting) {
    send(jp_comm, &dat, 1, 0);
    jp_waiting = 0;
  }

  count_comp = 1;
}

void jtag_init(const int port) {
  count_comp = 0;
  jp_waiting = 0;
  jp_got_con = 0;

  socket_port = port;

  socket_open();
}

// Checks to see if we got a connection
int jp_check_con()
{
  int ret;

  if((jp_comm = accept(jp_comm_m, NULL, NULL)) == -1) {
    if(errno == EAGAIN)
      return 0;

    fprintf(stderr, "Unable to accept connection: %s\n", strerror(errno));
    return 0;
  }


  // Set the comm socket to non-blocking.
  // Close the server socket, so that the port can be taken again
  // if the simulator is reset.
  ret = fcntl(jp_comm, F_GETFL);
  ret |= O_NONBLOCK;
  fcntl(jp_comm, F_SETFL, ret);
  close(jp_comm_m);

  printf("JTAG communication connected!\n");
  jp_got_con = 1;
  return 1;
}
