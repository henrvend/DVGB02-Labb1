#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT 4446
#define BUFFER_SIZE 4096

char *jpg = "jpeg";
char *gif = "gif";
char *html = "html";

void contType(char * type, int sd){
	if (!strcmp(type,jpg))
	{
		write(sd,"HTTP/1.1 200 OK\r\n",17);
		write(sd ,"Content-Type: image/jpeg\r\n", 26);  // 26 för JPEG 25 för GIF   
		write(sd,"\r\n",2);
	}
	if (!strcmp(type,gif))
	{
		write(sd,"HTTP/1.1 200 OK\r\n",17);
		write(sd ,"Content-Type: image/gif\r\n", 25);  // 26 för JPEG 25 för GIF   
		write(sd,"\r\n",2);
	}
	if (!strcmp(type,html))
	{
		write(sd,"HTTP/1.1 200 OK\r\n",17);
		write(sd ,"Content-Type: text/html\r\n", 25);  // 26 för JPEG 25 för GIF   
		write(sd,"\r\n",2);
	}
}


void sendResp(int sd, int a, char * type){
	if(a == 1)
	{
		contType(type, sd);
	}
}

int main()
{
	/* Declarations */
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int clientaddrlen;
	int request_sd, sd, fd;
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
		read(sd, buf, BUFFER_SIZE);
		printf("%s \n", buf);
		/*if (!fork())
		{*/
			/* Read data from socket and write it */
			read(sd, buf, BUFFER_SIZE);
			token = strtok(buf, " ");
			token = strtok(NULL," /");
			printf("token: %s\n", token);
			fd = open(buf, O_RDONLY); /* open the file to be sent back */		
			ContentToken = strtok(token,".");	/*Parse out the content type*/
			ContentToken = strtok(NULL,"");
			
			
			if(fd>0)
			{
				sendResp(sd, 0, ContentToken);
				while(1)
				{
					bytes = read(fd,buf,BUFFER_SIZE);
					if(bytes<=0) break;
					write(sd, buf, bytes);
				}
			}
			close(fd);
		/*}*/
	}

	/*Close sockets */
	close(sd);
	close(request_sd);
	printf("Both sockets are closed.");
}
