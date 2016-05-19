#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUFFSIZE 1024
#define PORT_NO 2234
#define MAX_WAITING_CLIENTS 10
#define SERVER_ADDR "127.0.0.1"
#define DEBUG 0

int fd;
int flags;
struct sockaddr_in server;
struct sockaddr_in client;
int server_size;
int client_size;
int bytes;
int err;
char on;
char buffer[BUFFSIZE+1];


char player_name[255];

int main(int argc, char* argv[]){
    printf("- - - - - - - - - - - - - - - - - - - - - - - -\n");
    printf("Kviz client\n");
    printf("- - - - - - - - - - - - - - - - - - - - - - - -\n");
    
    
    while(strlen(player_name) < 4){
        printf("Who are you? (Your nick sould be at least 3 characters.)\n>");
        fgets(player_name,sizeof(player_name),stdin);
    }
    printf("Hello %s\n",player_name);
    
    //networking
    on = 1;
    flags = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;
    client_size = sizeof client;
    bytes=BUFFSIZE;
    
    if(DEBUG)   printf("Creating socket...");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0){
        printf("ERROR: Socket cannot be created.\n");
        return -1;
    }
    if(DEBUG)   printf("...done\n");
    
    if(DEBUG)   printf("Set socket options...");
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);
    if(DEBUG)   printf("...done\n");
    
    if(DEBUG)   printf("Connecting...");
    err = connect(fd, (struct sockaddr *)&server, server_size);
    if(err < 0){
        printf("ERROR: Cannot connect to server\n");
        return -2;
    }
    if(DEBUG)   printf("...done\n");
    
    if(DEBUG)   printf("Sending...");
    
    strcpy(buffer, player_name);
    if(DEBUG)   printf("BUFF: %s\n",buffer);
    
    //bytes=strlen(buffer)+1;
    
    err = send(fd, buffer, bytes, flags);
    if(err < 0){
        printf("ERROR: Cannot send to the server\n");
        return -3;
    }
    if(DEBUG)   printf("...done\n");
    if(DEBUG)   printf("BUFF: %s\n",buffer);
    
    if(DEBUG)   printf("Reciving...");
    err = recv(fd, buffer, bytes, flags);
    if(err<0){
        printf("ERROR: Cannot receive from socket\n");
        return -4;
    }
    if(DEBUG)   printf("...done\n");
    //printf("BUFF: %s\n",buffer);
    
    
    printf("%s\n",buffer);
    
    //main game loop
    int no_rounds;
    if(DEBUG)   printf("Reciving...");
    err = recv(fd, buffer, bytes, flags);
    if(err<0){
        printf("ERROR: Cannot receive form socket\n");
        return -4;
    }
    no_rounds = atoi(buffer);
    printf("Number of rounds: %d\n",no_rounds);
    
    for(int round=0;round<no_rounds; ++round){
        printf("%d. Question:\n",round);
        err = recv(fd, buffer, bytes, flags);
        if(err<0){
            printf("ERROR: Cannot receive from socket\n");
            return -4;
        }
        printf("%s\n",buffer);
        printf(">");
        fgets(buffer,11,stdin);
              
        err = send(fd, buffer, bytes, flags);
        if(err < 0){
            printf("ERROR: Cannot send to the server\n");
            return -3;
        }
        
        if(DEBUG) printf("BUFF: %s",buffer);
        
        err = recv(fd, buffer, bytes, flags);
        if(err<0){
            printf("ERROR: Cannot receive from socket\n");
            return -4;
        }
        if(strstr(buffer,"Draw")!=NULL)
        printf("Draw!\n");
        else printf("%s scores.\n",buffer);
        
        
        
        
    }
    
    err = recv(fd, buffer, bytes, flags);
    printf("End of the game.\n");
    if(strstr(buffer,player_name)!=NULL)
    printf("- - - - - You won! - - - - -\n");
    else if(strstr(buffer,"Draw")!=NULL)
    printf("- - - - - Draw! - - - - -\n");
    else printf("- - - - - You lost the game! - - - - -\n");
    
    
    
    close(fd);
    return 0;
}