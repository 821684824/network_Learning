#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define ERR_EXIT(m) \
  do{ \
    perror(m); \
    exit(EXIT_FAILURE); \
  }while(0)

int main(void)
{
    int listenfd;
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
        ERR_EXIT("socket fail!");
   
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5888);
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //inet_aton("127.0.0.1", &servaddr.sin_addr);
   
  int on = 1;
  if(setsockopt(listenfd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on)) < 0)
       ERR_EXIT("setsockopt error!");

   if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof servaddr) < 0)
        ERR_EXIT("bind fail!");
   
   if(listen(listenfd, SOMAXCONN) < 0)
       ERR_EXIT("listen fail!");
   
   //client address
   struct sockaddr_in cliaddr;
   memset(&cliaddr, 0, sizeof cliaddr);
   socklen_t clilen = sizeof(cliaddr);
   int connfd;
   if((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
       ERR_EXIT("accept fail!");
   printf("connect from [%s : %d]\n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
   
   char recvbuf[1024];
   while(1)
   {
       memset(recvbuf, 0, sizeof(recvbuf));
       int ret = read(connfd, recvbuf, sizeof(recvbuf));
       fputs(recvbuf, stdout);
       write(connfd, recvbuf, ret);
   }
   close(connfd);
   close(listenfd);
   return 0;

}

