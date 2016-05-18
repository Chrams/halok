
#include <time.h>
#include "question.h"
#include "player.h"
#include "game.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> 





//---------------------------
//Definitions
//---------------------------
#define BUFFSIZE 1024
#define PORT_NO 2234
#define MAX_WAITING_CLIENTS 10


//---------------------------
//Variables
//---------------------------
FILE *fp;
//char buffer[255];
int qnum;
sPlayer player[2];

int surrender0, surrender1;

int fd;
    int fdc[2];
    int flags;
    struct sockaddr_in server;
    struct sockaddr_in client[2];
    unsigned int server_size;
    unsigned int client_size[2];
    int bytes;
    int err;
    char on;
    char buffer[BUFFSIZE+1];




//---------------------------
//enty point
//---------------------------
int main(int argc, char* argv[]){
   
    
    printf("Server started...\n");
    
    if(argc != 2){
        printf("ERROR: No arguments.\n");
        return 1;
    }
    
    int number_of_rounds = atoi(argv[1]);
    printf("Number of rounds: %d\n",number_of_rounds);
    
    
    
    fp = fopen("test.yolo","r");
    printf("File opened\n");
    
    
    
    //fgets(buffer, 255, (FILE*)fp);
    //printf("%s\n",buffer);
    memset(&buffer[0],0,sizeof(buffer));
    qnum = getNumberOfQuestions(buffer, 255, fp);
    printf("Number of Questions: %d\n",qnum);   
    
    sQuestion database[qnum];
    getQuestions(database, qnum, buffer, 255, fp);
    
    printf("End of file handling...\n");
    fclose(fp);
    
    for(int i=0; i<qnum; ++i){
        printf("%d: %s [%d]\n",i,database[i].text,database[i].answer);
    }
    
    //---------------------------
    //Networking
    //---------------------------
    on = 1;
    flags = 0;
    bytes = BUFFSIZE;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;
    client_size[0] = sizeof client[0];
    client_size[1] = sizeof client[1];
    
    
    
    printf("Creating socket...");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0){
        printf("ERROR: Socket cannot be created.\n");
        return -1;
    }
    printf("...done\n");
    
    
    printf("Setting up socket options...\n");
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
    printf("...SO_REUSEADDR set!...");
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);
    printf("...SO_KEEPALIVE set!...");
    printf("...done\n");
    
    printf("Binding...");
    err = bind(fd, (struct sockaddr *) &server, server_size);
    if(err<0){
        printf("ERROR: Cannot bind to the socket.\n");
        return -2;
    }
    printf("...done\n");
    
    printf("Listening...");
    err = listen(fd, MAX_WAITING_CLIENTS);
    if(err<0){
        printf("ERROR: Cannot listen to the socket.\n");
        return -3;
    }
    printf("...done\n");
    
    
    printf("Accepting...");
    fdc[0] = accept(fd, (struct sockaddr *) &client[0], &client_size[0]);
    if(fdc[0]<0){
        printf("Cannot accept on socket.\n");
        return -4;
    }
    printf("...done\n");
    
    printf("Reciving...");
    //memset(&buffer[0],0,BUFFSIZE);
    err = recv(fdc[0], buffer, bytes, flags);
    printf("...done\n");
    printf("BUFF: %s\n",buffer);
    
    
    printf("Message 0: %s\n", buffer);
    strcpy(player[0].nick, buffer);
    
    //TODO: Send "Waiting for other player" message
    //memset(&buffer[0],0,BUFFSIZE);
    sprintf(buffer,"Waiting for other player to join.");
    //bytes = strlen(buffer) + 1;
    printf("BUFF: %s\n",buffer);
    err = send(fdc[0], buffer, bytes, flags);
    if(err < 0){
        printf("ERROR: Cannot send to client\n");
        return -6;
    }
    
    fdc[1] = accept(fd, (struct sockaddr *) &client[1], &client_size[1]);
    if(fdc[1]<0){
        printf("Cannot accept on socket.\n");
        return -4;
    }
    
    //memset(&buffer[0],0,BUFFSIZE);
    err = recv(fdc[1], buffer, bytes, flags);
    printf("Message 1: %s\n", buffer);
    strcpy(player[1].nick, buffer);
    
    //memset(&buffer[0],0,BUFFSIZE);
    sprintf(buffer,"Ready to play.");
    //bytes = strlen(buffer) + 1;
    
    err = send(fdc[1], buffer, bytes, flags);
    if(err < 0){
        printf("ERROR: Cannot send to client\n");
        return -6;
    }
    
    
    
    //---------------------------
    //game logic
    //---------------------------
    srand(time(NULL));

    if(number_of_rounds>qnum){
        printf("WARNING: We don't have enough questions in the databank...");
        number_of_rounds=qnum;
        printf("... there will be only %d rounds.\n",number_of_rounds);
    }
    
    //zeroing out players
    player[0].answer = 0;
    player[0].score = 0;
    player[1].answer = 0;
    player[1].score = 0;
    
    
    
    sprintf(buffer,"%d", number_of_rounds);
    printf("BUFF: %s\n",buffer);
    err = send(fdc[0], buffer, bytes, flags);
    if(err < 0){
        printf("ERROR: Cannot send to client\n");
        return -6;
    }
    err = send(fdc[1], buffer, bytes, flags);
    if(err < 0){
        printf("ERROR: Cannot send to client\n");
        return -6;
    }
    
    
    
    
    //---------------------------
    //game loop
    //---------------------------
    for(int round=0;round<number_of_rounds; ++round){
        int r = rand() % qnum;
        while(database[r].used != 0)
            r=rand() % qnum;
        
        database[r].used = 1;
        
        
        printf("%d. Question:\n",round);
        printf("%s",database[r].text);
        
        sprintf(buffer,"%s",database[r].text);
        printf("BUFF: %s\n",buffer);
        err = send(fdc[0], buffer, bytes, flags);
        if(err < 0){
            printf("ERROR: Cannot send to client\n");
            return -6;
        }
        err = send(fdc[1], buffer, bytes, flags);
        if(err < 0){
            printf("ERROR: Cannot send to client\n");
            return -6;
        }
        
        surrender0 = 0;
        surrender1 = 0;
        
        err = recv(fdc[0], buffer, bytes, flags);
        /*if(strcmp(buffer,"feladom")==0){
            surrender0 = 1;
        }*/
        if(buffer[0]!='0' || buffer[0]!='1' ||
            buffer[0]!='2' || buffer[0]!='3' || buffer[0]!='4' ||
            buffer[0]!='5' || buffer[0]!='6' || buffer[0]!='7' ||
            buffer[0]!='8' || buffer[0]!='9' || buffer[0]!='-'){
                surrender0 = 1;
        }
        player[0].answer = atoi(buffer);
        printf("P1 BUFF: %d\n",atoi(buffer));
        
        
        err = recv(fdc[1], buffer, bytes, flags);
        /*if(strcmp(buffer,"feladom")==0){
            surrender1 = 1;
        }*/
        if(buffer[0]!='0' || buffer[0]!='1' ||
            buffer[0]!='2' || buffer[0]!='3' || buffer[0]!='4' ||
            buffer[0]!='5' || buffer[0]!='6' || buffer[0]!='7' ||
            buffer[0]!='8' || buffer[0]!='9' || buffer[0]!='-'){
                surrender0 = 1;
        }
        player[1].answer = atoi(buffer);
        printf("P2 BUFF: %s\n",buffer);
        
        printf("SUrrender status:\np1: %d\np2: %d\n",surrender0, surrender1);
        
        
        if(surrender0 == 1 && surrender1 == 0){
            sprintf(buffer, "%s scores\n", player[1].nick);
            player[1].score++;
        } else if(surrender0 == 0 && surrender1 == 1){
            sprintf(buffer, "%s scores\n", player[0].nick);
            player[0].score++;
        } else if(surrender0 == 1 && surrender1 == 1){
            sprintf(buffer,"Draw\n");
        } else {
            //matchmaking
            printf("The correct answer: %d\n",database[r].answer);
            for(int i=0;i<2;++i){
                printf("player[%d]: %d\n",i,player[i].answer);
            }
            
            int mm = matchMaking(player,database[r].answer);
            switch(mm){
                case 0:
                    sprintf(buffer, "%s scores\n", player[0].nick);
                    player[0].score++;
                    break;
                case 1:
                    sprintf(buffer, "%s scores\n", player[1].nick);
                    player[1].score++;
                    break;
                case -1:
                    sprintf(buffer,"Draw\n");
            }
        }
        
        err = send(fdc[0], buffer, bytes, flags);
        err = send(fdc[1], buffer, bytes, flags);
        
        
           
    }
    //---------------------------
    //end of game loop
    //---------------------------
    printf("player1: %d\nplayer2: %d\n",player[0].score, player[1].score);
    if(player[0].score > player[1].score){
        sprintf(buffer, "%s wins!",player[0].nick);
    } else if(player[1].score > player[0].score){
        sprintf(buffer, "%s wins!",player[1].nick);
    } else {
        sprintf(buffer, "Draw!");
    }
    
    err = send(fdc[0], buffer, bytes, flags);
    err = send(fdc[1], buffer, bytes, flags);
    
    close(fd);
    close(fdc[0]);
    close(fdc[1]);
    
    return 0;
}