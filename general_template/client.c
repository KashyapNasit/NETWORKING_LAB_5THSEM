#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define BUF_SIZE 100
int counter = 1;
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define clientString "\n[ %s%5d%s ] Client: "
#define PRINT_c(x) printf(clientString x,GREEN,counter++,NORMAL_COLOR);fflush(stdout)

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage : [Server IP] [PORT]\n");
		exit(1);
	}

	int sock = socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in si_server;

	si_server.sin_family      = AF_INET;
	si_server.sin_addr.s_addr = inet_addr(argv[1]);
	si_server.sin_port        = htons(atoi(argv[2]));
	memset(&(si_server.sin_zero),0,sizeof(si_server.sin_zero));
	socklen_t serv_len        = sizeof(si_server);

	connect(sock,(struct sockaddr *)&si_server, serv_len);
	printf("Connected to server at %s:%d\n",inet_ntoa(si_server.sin_addr),ntohs(si_server.sin_port));

	char buf[BUF_SIZE];
	int nBytes;

	nBytes = recv(sock,buf,sizeof(buf),0);
	PRINT_c("Recieved Bytes         : ");printf("%4dB",nBytes);
	PRINT_c("Server's Response      : ");printf("%s\n",buf);
	return 0;
}
