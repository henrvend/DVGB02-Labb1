#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void send_file(int client_sd, const char *filename, const char *content_type)
{
    FILE *file = fopen(filename, "rb");

    if (file != NULL)
    {
        // Send HTTP response header
        dprintf(client_sd, "HTTP/1.1 200 OK\r\nServer: Demo Web Server\r\nContent-Type: %s\r\n\r\n", content_type);

        // Send the content of the file
        char buffer[BUFFER_SIZE];
        size_t bytes_read;

        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
        {
            write(client_sd, buffer, bytes_read);
        }

        // Close the file
        fclose(file);
    }
    else
    {
        // File not found (404)
        const char *response_404 = "HTTP/1.1 404 Not Found\r\nServer: Demo Web Server\r\n\r\nFile not found";
        write(client_sd, response_404, strlen(response_404));
    }
}

int main()
{
    /* Declarations */
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen; // Ändrad till socklen_t
    int request_sd, sd;
    char buf[BUFFER_SIZE];
    int n;

    /* Create socket */
    request_sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    

    printf("Servers listening socket created. \n");

    /* Fill in the address structure */
    memset(&serveraddr, 0, sizeof(struct sockaddr_in));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);

    /* Bind address to socket */
    bind(request_sd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in));
    printf("Servers address binded to the socket. \n");

    /* Activate connect request queue */
    // UDP är ett connectionless protokoll: så listen behövs inte.
    // listen(request_sd, SOMAXCONN);

    while (1)
    {
        /* Receive connection */
        clientaddrlen = sizeof(struct sockaddr_in);
        //sd = accept(request_sd, (struct sockaddr *)&clientaddr, &clientaddrlen);
        n = recvfrom(request_sd, buf, sizeof(buf), 0, (struct sockaddr *)&clientaddr, &clientaddrlen);
        printf("Client connected. \n");

        printf("buf %s \n", buf);

        if (strstr(buf, "GET /test") != NULL)
        {
            // Get current time
            time_t current_time;
            struct tm *time_info;
            time(&current_time);
            time_info = localtime(&current_time);

            // Format time as a string
            char time_str[64];
            strftime(time_str, sizeof(time_str), "Current time: %Y-%m-%d %H:%M:%S", time_info);

            // Send the time back to the client
            write(sd, time_str, strlen(time_str));
        }

        else
        {
            // Unknown request
            const char *response_unknown = "HTTP/1.1 400 Bad Request\r\nServer: Demo Web Server\r\n\r\nUnknown request";
            write(sd, response_unknown, strlen(response_unknown));
        }

        /* Close the connection */
        //close(sd);
    }

    /* Close the listening socket */
    close(request_sd);

    printf("Both sockets are closed.");

    return 0;
}
