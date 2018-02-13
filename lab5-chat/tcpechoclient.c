#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv){
	char port[10];
	printf("Enter a port: ");
	fgets(port, 10, stdin);
	int portNum = atoi(port);

	char ip[100];
	printf("Enter an IP: ");
	fgets(ip, 100, stdin);
	strtok(ip, "\n");

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	fd_set sockets;  // a set of file descriptors
	FD_ZERO(&sockets); // initializes fd_set to have nothing in it
	if(sockfd<0){
		printf("There was an error creating the socket\n");
		return 1;
	}

	struct sockaddr_in serveraddr, clientaddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(portNum);
	serveraddr.sin_addr.s_addr=inet_addr(ip);

	int e = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(e<0){
		printf("There was an error connecting\n");
		return 1;
	}
	FD_SET(sockfd, &sockets); // adds socket to fd_set
	FD_SET(STDIN_FILENO, &sockets);

	while(1){

		int len=sizeof(clientaddr);
		fd_set tmp_set = sockets;  // make a copy of sockets for destructive operation
		select(FD_SETSIZE, &tmp_set, NULL, NULL, NULL); // changes data in tmp_set, the sockets we can read from will be left in tmp_set
		int i;
		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &tmp_set)) {
				if (i == sockfd) {
					int clientsocket = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
					FD_SET(clientsocket, &sockets);
				} else if (i == STDIN_FILENO) {
					char sendStr[5000];
					fgets(sendStr,5000,stdin);
					send(sockfd,sendStr,strlen(sendStr)+1,0);
					if (strcmp(sendStr, "quit\n") == 0) {
						printf("Quitting...\n");
						close(i);
						close(sockfd);
						FD_CLR(i, &sockets);
						exit(0);
					}
				} else {
					char receiveStr[5000];
					recv(i,receiveStr,5000,0);
					printf("Got from client: %s",receiveStr);
					if (strcmp(receiveStr, "quit\n") == 0) {
						printf("Quitting...\n");
						close(i);
						close(sockfd);
						FD_CLR(i, &sockets);
						exit(0);
					}
				}
			}
		}
	}
}
