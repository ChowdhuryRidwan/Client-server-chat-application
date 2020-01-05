//server side code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void error(const char *msg)
{
	perror(msg);
	exit(1); //try exit(0) after
}

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"Port number not provided. Program terminated.\n");
		exit(1); //try with exit(0);
	}
	
	int sockfd, newsockfd, portno, n;
	char buffer[255]; //used to send messages
	
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t client; //it is a 32 bit data type
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		error("Error opening socket.");
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno=atoi(argv[1]);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(portno);
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		error("Binding failed.");
	listen(sockfd,0);
	client=sizeof(cli_addr);
	newsockfd=accept(sockfd, (struct sockaddr *)&cli_addr, &client);
	if(newsockfd<0)
		error("Error on accept function.");
	while(1)
	{
		bzero(buffer, 255);
		n=read(newsockfd,buffer,255);
		if(n<0)
			error("Error in reading.");
		printf("Client: %s\n", buffer);
		bzero(buffer,255);
		fgets(buffer,255,stdin); //reads bytes from the stdin stream
		n=write(newsockfd,buffer,strlen(buffer));
		if(n<0)
			error("Error in writing.");
		int i=strncmp("Bye",buffer,3);
		if(i==0)
			break;
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
