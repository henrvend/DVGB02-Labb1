#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 4446
#define BUFFER_SIZE 25

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
	int clientaddrlen;
	int request_sd, sd;
	char buf[25];
	int n;

	/* Create socket */
	request_sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	printf("Servers listening socket created. \n");

	/* Fill in the address structure */
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(8080);
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

		/* Read data from socket and write it */
		read(sd, buf, 24);
		printf("%s", buf);

		int fd = open(buf, O_RDONLY); /* open the file to be sent back */
		
		while (1)
		{
			int bytes = read(fd, buf, BUFFER_SIZE); /* read from file */
			if (bytes <= 0) break; /* check for end of file */
			write(sd, buf, bytes);					/* write bytes to socket */
		}

		/*if (strstr(buf, "GET /index.html") != NULL) {
			printf("\nFungerar!\n");
			// Send index.html back to the client
			send_file(sd, "/sample_website/index.html", "text/html");
		} else if (strstr(buf, "GET /image.jpg") != NULL) {
			// Send image.jpg back to the client
			send_file(sd, "sample_website/image.jpg", "image/jpeg");
		} else {
			// Unknown request
			const char *response_unknown = "HTTP/1.1 400 Bad Request\r\nServer: Demo Web Server\r\n\r\nUnknown request";
			write(sd, response_unknown, strlen(response_unknown));
		}*/
	}

	/*Close sockets */
	close(sd);
	close(request_sd);
	printf("Both sockets are closed.");
}
