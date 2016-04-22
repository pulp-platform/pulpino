
#include "svdpi.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "dpiheader.h"


// messages look like this:
// WE(1-BIT)|0(3-BIT)|BE(4-BIT)|ADDR (32-bit)|WDATA(32-BIT)

int mem_check_con();

uint8_t mem_got_con;

static int mem_socket_m;
static int mem_socket_c;

int mem_socket_port;


void mem_socket_open() {
  struct sockaddr_in addr;
  int ret;

  mem_got_con = 0;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(mem_socket_port);
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  mem_socket_m = socket(PF_INET, SOCK_STREAM, 0);
  if(mem_socket_m < 0)
  {
    fprintf(stderr, "Unable to create comm socket: %s\n", strerror(errno));
    return;
  }

  int yes = 1;
  if(setsockopt(mem_socket_m, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    fprintf(stderr, "Unable to setsockopt on the socket: %s\n", strerror(errno));
    return;
  }

  if(bind(mem_socket_m, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    fprintf(stderr, "Unable to bind the socket: %s\n", strerror(errno));
    return;
  }

  if(listen(mem_socket_m, 1) == -1) {
    fprintf(stderr, "Unable to listen: %s\n", strerror(errno));
    return;
  }

  ret = fcntl(mem_socket_m, F_GETFL);
  ret |= O_NONBLOCK;
  fcntl(mem_socket_m, F_SETFL, ret);

  fprintf(stderr, "Listening on port %d\n", mem_socket_port);
}

int mem_poll(mem_packet_t* packet) {
  char data[9];
  int ret;

  if(!mem_got_con) {
    if(!mem_check_con()) {
      return 0;
    }
  }

  ret = recv(mem_socket_c, data, 9, 0);

  // check connection abort
  if((ret == -1 && errno != EWOULDBLOCK) || (ret == 0)) {
    printf("Memory connection closed\n");

    close(mem_socket_c);
    mem_socket_open(mem_socket_port);
    return 0;
  }

  // no available data
  if(ret == -1 && errno == EWOULDBLOCK)
    return 0;

  if (ret != 9) {
    printf("Well, not a complete packet then...\n");
    return 0;
  }

  packet->we    = (data[0] & 0x80) == 0x80;
  packet->be    =  data[0] & 0x0F;
  packet->addr  =  ntohl(*((int*)&data[1]));
  packet->wdata =  ntohl(*((int*)&data[5]));

  printf("Request for Address %08X, Wdata %08X, Write %d, BE %X\n", packet->addr, packet->wdata, packet->we, packet->be);

  return 1;
}

void mem_push(const mem_packet_t* packet) {
  int ret;
  uint32_t data;

  data = htonl(packet->rdata);
  ret = send(mem_socket_c, &data, 4, 0);
  if (ret != 4) {
    printf ("Unable to send\n");
  }
}

void mem_init(const int port) {
  mem_got_con = 0;

  mem_socket_port = port;

  mem_socket_open(port);
}

// Checks to see if we got a connection
int mem_check_con()
{
  int ret;

  if((mem_socket_c = accept(mem_socket_m, NULL, NULL)) == -1) {
    if(errno == EAGAIN)
      return 0;

    fprintf(stderr, "Unable to accept connection: %s\n", strerror(errno));
    return 0;
  }


  // Set the comm socket to non-blocking.
  // Close the server socket, so that the port can be taken again
  // if the simulator is reset.
  ret = fcntl(mem_socket_c, F_GETFL);
  ret |= O_NONBLOCK;
  fcntl(mem_socket_c, F_SETFL, ret);
  close(mem_socket_m);

  printf("Memory communication connected!\n");
  mem_got_con = 1;
  return 1;
}
