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

#define LISTENQ 5
#define SA struct sockaddr
#define MAXSIZE 50

struct emaildb{
    char address[MAXSIZE];
    char name[MAXSIZE];
};

struct emaildb d[MAXSIZE];

void fillDb(){
    strcpy(d[0].address,"harris@ics.uci.edu");
    strcpy(d[0].name,"Ian Harris");
    strcpy(d[1].address,"joe@cnn.com");
    strcpy(d[1].name,"Joe Smith");
    strcpy(d[2].address,"jane@slashdot.org");
    strcpy(d[2].name,"Jane Smith");
}

void updateBufMessage(char * msgBuffer, char * msgStr){
    msgBuffer[0] = strlen(msgStr);
    int i;
    for (i = 0; i < strlen(msgStr); i++){
        msgBuffer[i+1] = msgStr[i];
    }
    msgBuffer[i+1] = '\0';
}

int open_listenfd(int port)
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error creating/opening socket\n");
        return -1;
    }
   
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) < 0){
        return -1;
    }
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0){
        printf("Error on binding\n");
        return -1;
    }

    if (listen(listenfd, LISTENQ) < 0){
        printf("Error on listening\n");
        return -1;
    }
    
    return listenfd;
}

char * findName(char * email){
    int i;
    char * name = NULL;
    for (i = 0; i < 3; i++){
        if (strcmp(d[i].address, email) == 0){
            name = d[i].name;
            break;
        }
    }
    name += '\0';
    return name;
}

int main(int argc, char * argv[]) {
    fillDb();
    int listenfd, connfd, port, n;
    char * name;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char bufferReadFromClient[256];
    if (argc < 2){
        printf("ERROR: insufficient amount of arguments provided\n");
        exit(1);
    }
    port = atoi(argv[1]);
    listenfd = open_listenfd(port);
    printf("Address server has started\n");
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        if (connfd < 0){
            printf("Error on accept\n");
            exit(1);
        }
        bzero(bufferReadFromClient, 256);
        while (read(connfd, bufferReadFromClient,255) > 0) {
            name = findName(&bufferReadFromClient[1]);
            if (name == NULL){
                printf("%s\n", &bufferReadFromClient[1]);
                updateBufMessage(bufferReadFromClient, "unknown");
                write(connfd, bufferReadFromClient, strlen(bufferReadFromClient));
            }
            else {
                printf("%s\n", &bufferReadFromClient[1]);
                updateBufMessage(bufferReadFromClient, name);
                write(connfd, &bufferReadFromClient[0], strlen(bufferReadFromClient));
            }
        }
        close(connfd);
    }
    return 0;
}




// use bzero to clear out the bufferReadFromClient