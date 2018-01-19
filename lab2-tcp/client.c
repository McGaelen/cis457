#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char portNumber[10];
    char ipAddress[20];
    char filename[5000];
    FILE *newFile;
    int filesize;
    char fileContents[5000];

    printf("Which port number? ");
    fgets(portNumber, 10, stdin);
    printf("What IP address? ");
    fgets(ipAddress, 20, stdin);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("There was an error creating the socket\n");
        return 1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(portNumber));
    serveraddr.sin_addr.s_addr = inet_addr(ipAddress);

    int e = connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
    if (e < 0) {
        printf("There was an error connecting\n");
        return 1;
    }

    printf("Enter a filename: ");
    fgets(filename, 5000, stdin);
    strtok(filename, "\n");
    send(sockfd, filename, strlen(filename) + 1, 0);

    recv(sockfd, fileContents, 5000, 0);
    newFile = fopen("outFile.txt", "w");
    fwrite(fileContents, strlen(fileContents), 1, newFile);

    close(sockfd);

    return 0;

}
