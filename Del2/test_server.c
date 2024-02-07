#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <time.h>

#define PORT 37
#define MAXLINE 1000 

  
// Driver code 
int main() 
{    
    char buffer[100]; 
    time_t t;
    time(&t);
    int listenfd, len; 
    struct sockaddr_in servaddr, cliaddr; 
    
    memset(&servaddr, 0, sizeof(servaddr)); 
  
    // Create a UDP Socket 
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
          
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET;  
   
    // bind server address to socket descriptor 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
       
    //receive the datagram 
    len = sizeof(cliaddr); 

    //receive message from server 
    int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,&len);
    buffer[n] = '\0'; 
    puts(buffer); 
           
    // send the response 
    sendto(listenfd, ctime(&t), MAXLINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
} 