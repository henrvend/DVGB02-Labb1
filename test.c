#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10
char *jpg = "jpeg";
char *gif = "gif";
char *html = "html";

void SendContentType(char * type, int socket){
	if (!strcmp(type,jpg))
	{
		write(socket,"HTTP/1.1 200 OK\r\n",17);
		write(socket ,"Content-Type: image/jpeg\r\n", 26);  // 26 för JPEG 25 för GIF   
		write(socket,"\r\n",2);
	}
	if (!strcmp(type,gif))
	{
		write(socket,"HTTP/1.1 200 OK\r\n",17);
		write(socket ,"Content-Type: image/gif\r\n", 25);  // 26 för JPEG 25 för GIF   
		write(socket,"\r\n",2);
	}
	if (!strcmp(type,html))
	{
		write(socket,"HTTP/1.1 200 OK\r\n",17);
		write(socket ,"Content-Type: text/html\r\n", 25);  // 26 för JPEG 25 för GIF   
		write(socket,"\r\n",2);
	}
}


void SendHTTPresp(int sa, int a, char * type){
	if (a == 1)
	{
		SendContentType(type,sa);

	}
	else{
		write(sa,"HTTP/1.1 404 Not Found\r\n",22);
		write(sa ,"Content-Type: text/html\r\n", 26);   
		write(sa,"\r\n",2);
	}
}

int main(int argc, char const *argv[])
{
	int s,b,l,fd,error,sa,bytes, on = 1;
	char buf[BUF_SIZE];				/*buffer for outgoing file*/
	struct sockaddr_in channel;		/*holds IP adress */
	char * token;
	char * ContToken;

	/* Build adress structure to bind to socket */
	memset(&channel,0,sizeof(channel));
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);

	/* Passive open. Wait for connection. */
	s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); /* create socket*/
	if(s < 0) perror("socket failed");
	setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on));

	b = bind(s, (struct sockaddr *)&channel,sizeof(channel));
	if(b < 0) perror("bind failed");

	l = listen(s,QUEUE_SIZE);
	if(l < 0) perror("listen failed");




	/* Socket is now set up and bound. Wait for connection and process it. */
	while(1)
	{
		sa = accept(s,0,0);		/*Block for connection request*/
		if (sa < 0) perror("accept failed");

		if (!fork())
		{
			close(s);    
		    read(sa,buf,BUF_SIZE);
		    token = strtok(buf," ");		/*Parse out the file we are looking for*/
		    token = strtok(NULL," /");
		    printf("%s\n",token );
		    fd = open(token,O_RDONLY);
		    ContToken = strtok(token,".");	/*Parse out the content type*/
		    ContToken = strtok(NULL,"");
		    printf("%s\n",ContToken );
		    if(fd > 0){
		    	SendHTTPresp(sa,1,ContToken);
		    	while(1){
		   			bytes = read(fd,buf,BUF_SIZE);
		   				if (bytes <= 0) break;
		   			write(sa,buf,bytes);
		   		}	
		    }
		    else{
		    	SendHTTPresp(sa,0,ContToken);
		    	error = open("404.html",O_RDONLY);
		    	while(1){
		   			bytes = read(error,buf,BUF_SIZE);
		   			if (bytes <= 0) break;
		   			write(sa,buf,bytes);
		   		}
		    }
		   
		   	close(fd);
		    exit(0);
		}
		close(sa);
	}

}