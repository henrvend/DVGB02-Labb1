#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>

#define PORT 37
#define BUFFER_SIZE 2000
#define MAXLINE 1000

int main()
{
    /* Declarations */
    char buffer[BUFFER_SIZE];
    int sockfd, n;
    struct sockaddr_in serveraddr, clientaddr;

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    socklen_t clientaddrlen;
    
    while (1)
    {
        /* Receive data */
        clientaddrlen = sizeof(clientaddr);
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &clientaddrlen);
        printf("Received from client: %s\n", buffer);

        // Here you can process the received data or send a response back to the client
        // For example, sending a response back to the client
        char *response = "Hello Client";
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)&clientaddr, clientaddrlen);
    }

    close(sockfd);

    return 0;
}
