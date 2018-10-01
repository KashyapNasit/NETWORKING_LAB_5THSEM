#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define WAITING_SIZE 10
#define BUFF_SIZE 1000

// ITERATIVE TCP SERVER FILE

int counter = 1;
int query = 0;
#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define serverString "\n[ %s%5d%s ] Server: "
#define PRINT_c(x) printf(serverString x,GREEN,counter++,NORMAL_COLOR);fflush(stdout)


//////////////////////////////////////////////////////
//
// write the definition of the driver : 			//
// YOUR RESPONSIBILITY to set buff[nBytes] = '\0'	//



// Sample driver function
char * driver(void)
{
	char *Buf = malloc(sizeof(char)*BUFF_SIZE);
	time_t tick = time(NULL);

	int FileHandler;
	char FileBuffer[1024];

	FileHandler = open("/proc/loadavg", O_RDONLY);
	if(FileHandler < 0)
		perror("could not open file");

	read(FileHandler, FileBuffer,16);
	FileBuffer[15] = '\0';

	snprintf(Buf,BUFF_SIZE,"\n\t%s\tLoad : %s",ctime(&tick),FileBuffer);
	close(FileHandler);
	
	return Buf;
}


//
//////////////////////////////////////////////////////


int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage : [PORT]\n");
		exit(1);
	}
	int sock = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in si_me,si_client;

	si_me.sin_family      = AF_INET;
	si_me.sin_port        = htons(atoi(argv[1]));
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(si_me.sin_zero),0,sizeof(si_me.sin_zero));

	socklen_t me_len      = sizeof(si_me);
	socklen_t client_len  = sizeof(si_client);


	// Bind the socket
	bind(sock,(struct sockaddr *)&si_me, me_len);

	//start listening
	listen(sock,WAITING_SIZE);


	int nBytes;

	while(1)
	{
		int acptd_sock = accept(sock,(struct sockaddr *)&si_client,&client_len);

		PRINT_c("Connected to Client ");
		printf("[ %s:%d ]",inet_ntoa(si_client.sin_addr),ntohs(si_client.sin_port));
		fflush(stdout);
	
		char *Buf = driver();

		nBytes = send(acptd_sock,Buf,strlen(Buf),0);
		if (nBytes <= 0)
		{
			PRINT_c("Client Left");
			break;
		}

		PRINT_c("Sent Bytes : ");printf("%4dB",nBytes);
		PRINT_c("Responed Succesfully");printf(" Request# %d",++query);fflush(stdout);
		close(acptd_sock);
	}

	close(sock);
	return 0;
}
