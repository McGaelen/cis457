#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

void *handleclient(void *arg) {
    int clientsocket = *(int *)arg; // cast pointer as int pointer, then dereference
    char line[5000];
    recv(clientsocket,line,5000,0);
    printf("Got from client: %s\n",line);
    send(clientsocket,line,strlen(line)+1,0);
    close(clientsocket);
}

int main(int argc, char **argv){
  int sockfd = socket(AF_INET,SOCK_STREAM,0);

  struct sockaddr_in serveraddr,clientaddr;
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_port=htons(9876);
  serveraddr.sin_addr.s_addr=INADDR_ANY;

  bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
  listen(sockfd,10);

  while(1){
    int len=sizeof(clientaddr);
    int clientsocket = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
    pthread_t child;
    pthread_create(&child, NULL, handleclient, &clientsocket);
    pthread_detach(child);
  }
}
