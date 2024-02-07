#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 37
#define BUFFER 1000

int main(int argc, char *argv[])
{
    int sockfd;
    uint32_t net_time;
    time_t cur_time;
    struct sockaddr_in servaddr;
    char time[BUFFER];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Clear server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Send empty datagram to server
    sendto(sockfd, NULL, 0, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive time datagram from server with "recvfrom"
    recvfrom(sockfd, &net_time, sizeof(net_time), 0, NULL, NULL);

    // Convert network byte order to host byte order
    cur_time = ntohl(net_time);
    // Convert host byte order to string
    strcpy(time, ctime(&cur_time));

    // Print time received from server
    printf("%s", time);

    // Close the socket
    close(sockfd);

    return 0;
}
