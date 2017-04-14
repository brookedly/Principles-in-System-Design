//Brooke Ly brookedl 33256705
//Sunaina Kumar sunainak 27947698

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1024
#define SA struct sockaddr

void constructMsgBuffer(char * serverBuffer, char * email){
    serverBuffer[0] = strlen(email);
    int i;
    for (i = 0; i < strlen(email); i++){
        serverBuffer[i+1] = email[i];
    }
    serverBuffer[i+1] = '\0';
}

int open_clientfd(char *hostname, int port) {
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Could not create socket\n");
        return -1;
    }
    
    if ((hp = gethostbyname(hostname)) == NULL){
        printf("ERROR: no such host\n");
        return -2;
    }
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);
    
    if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0){
        printf("ERROR: connection error\n");
        return -1;
    }
    return clientfd;
}

int main(int argc, char * argv[]) {
    char serverSendMsgBuffer[256];
    int clientfd, port;
    int n;
    char *host;
    char buffer[256];
    if (argc < 3){
        printf("ERROR: insufficient amount of arguments provided\n");
        exit(1);
    }
    host = argv[1];
    port = atoi(argv[2]);
    clientfd = open_clientfd(host, port);
    while (1){
        printf("> ");
        //scanf("%s", buffer);
        fgets(buffer, 255, stdin);
        size_t ln = strlen(buffer) -1;
        if (buffer[ln] == '\n'){
            buffer[ln] = '\0';
        }
        if (strcmp(buffer, "+++") == 0 ){
            close(clientfd);
            exit(0);
        }
        else {
            constructMsgBuffer(serverSendMsgBuffer, buffer);
            n = write(clientfd, serverSendMsgBuffer, 255);
            if (n < 0){
                printf("Error writing to a socket\n");
            }
            n = read(clientfd, buffer, 255);
            
            if (n < 0){
                printf("Error reading from a socket\n");
            }
            buffer[buffer[0]+1] = '\0';
            fputs(&buffer[1], stdout);
            fputs("\n", stdout);
            
        }
    }
    return 0;
}
