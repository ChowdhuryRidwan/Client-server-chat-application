//here we take 3 arguements. argv[0]=filename. argv[1]=server_ipaddress. argv[2]=portno
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> //stores hostent structure which hold info about a given host like host name and ipv4 address

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[255];
	
	if(argc<3)
	{
		fprintf(stderr,"usage %s hostname port\n",argv[0]);
		exit(1);
	}
	
	portno=atoi(argv[2]);
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		error("Error opening socket!!!");
	}
	server=gethostbyname(argv[1]); //The gethostbyname() function returns a structure of type hostent for the given host name. Here name is either a hostname, or an IPv4 address in standarddotnotatio
	if(server==NULL)
	{
		fprintf(stderr,"Error, no such host");
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	//h_length-> this is the length in bytes of each address. h_addr-> this is a synonym for h_addr_list[0]; in other words, it is the first host address.
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //this function copies info from server to the structure serv_addr
	serv_addr.sin_port=htons(portno);
	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
		error("Connection Failed.");
	while(1)
	{
		bzero(buffer,255);
		fgets(buffer,255,stdin);
		n=write(sockfd,buffer,strlen(buffer));
		if(n<0)
			error("Error on writing");
		bzero(buffer,255);
		n=read(sockfd,buffer,255);
		if(n<0)
			error("Error in reading");
		printf("Server: %s",buffer);
		int i=strncmp("Bye",buffer,3); //comparing Bye with the word in buffer. 3 is the length of word
		if(i==0)
			break;
	}
	
	close(sockfd);
	return 0;
}

