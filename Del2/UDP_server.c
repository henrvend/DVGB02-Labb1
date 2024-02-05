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
    char buffer[100];
    char *message = "Hello Server";
    int sockfd, n;
    struct sockaddr_in serveraddr;

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(PORT);
    printf("Testar UDP 1\n");
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(connect(sockfd,(struct sockaddr * )&serveraddr, sizeof(serveraddr)) > 0)
    {
        printf("\n Error: Connect failed \n");
        exit(0);
    }
    
    sendto(sockfd, message, MAXLINE, 0 , (struct sockaddr*)NULL, sizeof(serveraddr));

    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
    printf("Testar UDP");
    puts(buffer);

    close(sockfd);

}