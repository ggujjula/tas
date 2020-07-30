#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tas_ll.h>
#include <unistd.h>
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <errno.h>
*/

int main(void){
  int err = 0;
  struct flextcp_context my_context;
  struct flextcp_connection my_conn;
  struct flextcp_event my_event;

  err = flextcp_init();
  if(err < 0){
    fprintf(stderr, "Failed to init flextcp: %d\n", err);
    exit(-1);
  }
  printf("flextcp initialized\n");
  
  err = flextcp_context_create(&my_context);
  if(err < 0){
    fprintf(stderr, "Failed to create context: %d\n", err);
    exit(-1);
  }
  printf("Created context\n");

  err = flextcp_connection_open(&my_context, &my_conn, 0x0a000002, 41873);
  if(err < 0){
    fprintf(stderr, "Failed to start conn open op: %d\n", err);
    exit(-1);
  }
  printf("Requested conn open\n");

  while(flextcp_context_poll(&my_context, 1, &my_event) != 1);
  printf("Polled for conn open\n");

  if(my_event.event_type != FLEXTCP_EV_CONN_OPEN){
    fprintf(stderr, "Wrong event type: %d Should be %d\n", my_event.event_type, FLEXTCP_EV_CONN_OPEN);
    exit(-1);
  }  
  if(my_event.ev.conn_open.status != 0){
    fprintf(stderr, "Bad event status: %d\n", my_event.ev.conn_open.status);
    exit(-1);
  }

  sleep(10);

  char *buf_ptr = NULL;
  int msg_len = 12;
  if(flextcp_connection_tx_alloc(&my_conn, msg_len, (void **)(&buf_ptr)) != msg_len){
    fprintf(stderr, "Could not alloc space for test tx\n");
    exit(-1);
  }
  printf("Alloced space at %p for test tx\n", buf_ptr);
  
  strcpy(buf_ptr, "Hello World\n");
  if(flextcp_connection_tx_send(&my_context, &my_conn, msg_len)){
    fprintf(stderr, "Could not send test tx data\n");
    exit(-1);
  }
  printf("Sent test tx data across %p\n", &my_conn);

  while(flextcp_context_poll(&my_context, 1, &my_event) != 1);
  printf("Got an event\n");

  while(1);
  //sleep(10);

  err = flextcp_connection_close(&my_context, &my_conn);
  if(err < 0){
    fprintf(stderr, "Failed to start conn close op: %d\n", err);
    exit(-1);
  }
  printf("Requested conn close\n");

  while(flextcp_context_poll(&my_context, 1, &my_event) != 1);
  printf("Polled for conn close\n");

  if(my_event.event_type != FLEXTCP_EV_CONN_CLOSED){
    fprintf(stderr, "Wrong event type: %d\n", my_event.event_type);
    exit(-1);
  }  
  
}
