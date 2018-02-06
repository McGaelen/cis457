#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// gaelen was here
int main(int argc, char **argv) {
    char portNumber[10];
    char filename[5000];
    FILE *f;
    long filesize;
    char fileContents[5000];
    char payload[5000];

    printf("Which port number? ");
    fgets(portNumber, 10, stdin);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr, clientaddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(portNumber));
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
    listen(sockfd, 10);

    while (1) {
        int len = sizeof(clientaddr);
        int clientsocket = accept(sockfd, (struct sockaddr *) &clientaddr, &len);
        filename[0] = '\0';
        fileContents[0] = '\0';
        payload[0] = '\0';

        recv(clientsocket, filename, 5000, 0);

        printf("Searching for file %s\n", filename);
        f = fopen(filename, "r");
        if (f != NULL) {
            fseek(f, 0, SEEK_END);
            filesize = ftell(f);
            rewind(f);
            fread(fileContents, filesize, 1, f);
            strcat(payload, fileContents);
            send(clientsocket, payload, strlen(payload) + 1, 0);
            printf("Found file %s.\n", filename);
        } else {
            perror("File open error");
            send(clientsocket, "Couldn't find file", 18 + 1, 0);
        }

        close(clientsocket);
    }
}
