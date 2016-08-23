#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
   do{   \
      perror(m); \
      exit(EXIT_FAILURE); \
   }while(0)

int main(void)
{
    int connfd;
    if((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket fail!");
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5888);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(connfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect fail!");
    
    char sendbuf[1024];
    char recvbuf[1024];
    while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        write(connfd, sendbuf, sizeof(sendbuf));
        read(connfd, recvbuf, sizeof(recvbuf));

        fputs(recvbuf, stdout);

    }

    close(connfd);
    return 0;
}
    
