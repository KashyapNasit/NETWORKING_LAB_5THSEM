#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>


#define WAIT_LOG 5
#define BUF_SIZE 1000

int main(int argc, char const *argv[])
{
    
    if (argc != 2)
    {
        printf("Usage [port]\n");
        return 0;
    }

    // a file descriptor to point to server's struct
    int server_sock = socket(AF_INET,SOCK_STREAM,0);

    // an address structure to store server's network identity
    struct sockaddr_in server,client;
    
    // initialize details
    // htons : host to network byte short
    // htonl : host to network byte long
    server.sin_family      = AF_INET;
    server.sin_port        = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(server.sin_zero),0,sizeof(server.sin_zero));

    socklen_t server_len = sizeof(server);
    socklen_t client_len = sizeof(client);

    // binding the socket to the server's struct
    bind(server_sock,(const struct sockaddr*)&server,server_len);

    // listen for any incoming connections
    listen(server_sock,WAIT_LOG);

    printf("Server up and listening....\n");
    // creates a new socket to handle this "client" request
    // and the old server_sock keeps listening for more connections
    int cloned_sock = accept(server_sock,(struct sockaddr *)&client, &client_len);

    char buf[BUF_SIZE];

    int recvd_bytes = recv(cloned_sock,buf,BUF_SIZE,0);
    printf("Client sent : %s",buf);
    int sent_bytes  = send(cloned_sock,buf,recvd_bytes,0);

}

