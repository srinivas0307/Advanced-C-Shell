#include "headers.h"

int iman(char * command){
    int sockfd;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error opening socket");
        return 0;
    }
    struct hostent *server=gethostbyname("man.he.net");
    if(server==NULL){
        fprintf(stderr,"No such host\n");
        return 0;
    }
    struct sockaddr_in server_addr;
    memset((char*)&server_addr,0,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    memcpy((char*)&server_addr.sin_addr.s_addr,(char*)server->h_addr,server->h_length);
    server_addr.sin_port=htons(80);
    if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        perror("Error connecting");
        return 0;
    }
    char request[max_size];
    snprintf(request,sizeof(request),"GET /?topic=%s&section=all HTTP/1.1\r\n""Host: man.he.net\r\n""Connection: close\r\n\r\n",command);
    if(write(sockfd,request,strlen(request))<0){
        perror("Error writing to socket");
        return 0;
    }
    char response[max_size];
    int bytes_read;
    int header_ended=0;
    while((bytes_read=read(sockfd,response,sizeof(response)-1))>0){
        response[bytes_read]='\0';
        int f=0;
        for(int i=0;i<bytes_read;i++){
            if(response[i]=='<'){
                f=0;
            }
            if(f==1){
                printf("%c",response[i]);
            }
            if(response[i]=='>'){
                f=1;
            }
        }
    }
    close(sockfd);
    return 0;
}