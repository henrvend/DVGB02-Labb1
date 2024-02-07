#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 37

int main() {
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    time_t cur_time;
    uint32_t net_time;
    while(1){
        // Create a UDP socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("Failed to create socket");
            exit(EXIT_FAILURE);
        }

        // Clear server address structure
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(PORT);
        
        // Bind the socket with the server address
        if (bind(sockfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
            perror("Failed to bind");
            exit(EXIT_FAILURE);
        }
        
        // Receive empty datagram from client with "recvfrom"
        int cliaddrlen = sizeof(clientaddr);
        recvfrom(sockfd, NULL, 0, 0, (struct sockaddr *)&clientaddr, &cliaddrlen);

        // Get current time
        if(!time(&cur_time)){
            close(sockfd);
            exit(0);
        }
        net_time = htonl((uint32_t)cur_time);

        // Send time as a 32 bit binary number to client with "sendto".
        if(sendto(sockfd, &net_time, sizeof(net_time), 0, (const struct sockaddr *)&clientaddr, sizeof(clientaddr))==-1)
        {
            printf("Could not send data.\n");
            exit(0);
        }

        // Close the socket
        close(sockfd);
    }

    return 0;
}
