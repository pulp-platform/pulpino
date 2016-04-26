
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

void mem_reopen() {
  close(mem_socket_c);
  mem_socket_open(mem_socket_port);
}

int mem_poll(mem_packet_t* packet, char* buffer) {
  char header[9];
  int ret;
  int received;

  if(!mem_got_con) {
    if(!mem_check_con()) {
      return -1;
    }
  }

  memset(header, 0, sizeof(header));

  ret = recv(mem_socket_c, header, 9, 0);

  // check connection abort
  if((ret == -1 && errno != EWOULDBLOCK) || (ret == 0)) {
    printf("Memory connection closed\n");

    mem_reopen();
    return -1;
  }

  // no available data
  if(ret == -1 && errno == EWOULDBLOCK)
    return -1;

  if (ret != 9) {
    printf("ERROR: Well, not a complete packet then..., size is only %d\n", ret);
    return -1;
  }

  packet->we   = (header[0] & 0x01) == 0x01;
  packet->addr =  *((int*)&header[1]);
  packet->size =  *((int*)&header[5]);

  // printf("Request for Address %08X, Write %d, size %d\n", packet->addr, packet->we, packet->size);

  if (packet->we) {
    if(buffer == NULL) {
      printf("ERROR: Could not allocate buffer\n");
      return -1;
    }

    received = 0;
    while (received != packet->size) {
      ret = recv(mem_socket_c, &buffer[received], packet->size - received, 0);

      // check connection abort
      if((ret == -1 && errno != EWOULDBLOCK) || (ret == 0)) {
        printf("Memory connection closed\n");

        mem_reopen();
        return -1;
      }

      if(ret == -1 && errno == EWOULDBLOCK)
        continue;

      received += ret;
    }
  }

  return 0;
}

int mem_push(const mem_packet_t* packet, const char* buffer) {
  int ret;
  char header[8];

  if (packet->we) {
    // write
    *((int*)&header[0]) = 0; // OK
    *((int*)&header[1]) = 0; // size

    ret = send(mem_socket_c, header, 5, 0);
    if (ret != 5) {
      printf ("Unable to send header\n");
      return -1;
    }
  } else {
    // read
    *((int*)&header[0]) = 0; // OK
    *((int*)&header[1]) = packet->size; // size

    ret = send(mem_socket_c, header, 5, 0);
    if (ret != 5) {
      printf ("Unable to send header\n");
      return -1;
    }

    if(buffer == NULL) {
      printf("ERROR: Could not get buffer\n");
      return -1;
    }

    ret = send(mem_socket_c, buffer, packet->size, 0);
    if (ret != packet->size) {
      printf ("Unable to send data\n");
      return -1;
    }
  }

  return 0;
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
