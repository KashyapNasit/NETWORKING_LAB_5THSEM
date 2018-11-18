#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>

#define BUF_SIZE 1000

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage [Server IP] [PORT]");
        return 0;
    }

    int client_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    // converts the dot and number IP to network byte order binary number
    server.sin_addr.s_addr = inet_addr(argv[1]);
    memset(&server.sin_zero,0,sizeof(server.sin_zero));

    socklen_t server_len = sizeof(server);

    // connects the client_socket to the address struct with network identity of the server
    connect(client_socket,(const struct sockaddr* )&server,server_len);

    printf("Connected to server....");
    char buf[BUF_SIZE];
    scanf("%s",buf);
    int sent_bytes = send(client_socket,buf,sizeof(buf),0);
    int recvd_bytes = recv(client_socket,buf,sizeof(buf),0);
    printf("Server Sent : %s",buf);

    return 0;
}

