#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 4096


void send_file(int client_sd, const char *filename, char *content_type)
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
	int clientaddrlen;
	int request_sd, sd, fd, s, on = 1;
	int bytes;
	char buf[BUFFER_SIZE];
	char * ContentToken;
	char * token;
	int n;

	/* Create socket */
	request_sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	printf("Servers listening socket created. \n");

	/* Fill in the address structure */
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(8080);


	if(request_sd < 0) perror("socket failed");
	setsockopt(request_sd,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on));

	/* Bind address to socket */
	bind(request_sd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in));
	printf("Servers address binded to the socket. \n");

	/* Activate connect request queue */
	listen(request_sd, SOMAXCONN);
	
	while (1)
	{
		/* Receive connection */
		clientaddrlen = sizeof(struct sockaddr_in);
		sd = accept(request_sd, (struct sockaddr *)&clientaddr, &clientaddrlen);
		printf("Client connected. \n");
		read(sd, buf, BUFFER_SIZE);
		printf("buf:: %s \n", buf);
		//if (!fork())
		//{
			/* Read data from socket and write it */
			token = strtok(buf, " ");
			token = strtok(NULL," ");
			printf("token: %s\n", token);
			memmove(token, token+1, strlen(token));
			printf("token: %s\n", token);
			char temp[BUFFER_SIZE];
			strcpy(temp, token);
			printf("temp: %s\n", temp);
			ContentToken = strtok(temp,".");	/*Parse out the content type*/
			ContentToken = strtok(NULL,"");
			printf("cont token: %s\n", ContentToken);
			send_file(sd, token, ContentToken);
			printf("Testar");			
		//}
	}

	/*Close sockets */
	close(sd);
	close(request_sd);
	printf("Both sockets are closed.");
}
