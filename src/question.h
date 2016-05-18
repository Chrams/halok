/* question.h */
#ifndef QUESTION_H
#define QUESTION_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//---------------------------
//typedefs
//---------------------------
typedef struct sQuestion{
    char text[255];
    int answer;
    int used;
} sQuestion;

//---------------------------
//functions
//---------------------------

char* readLine(char* buffer, int buff_size, FILE* fp){
    memset(&buffer[0],0,buff_size);
    fgets(buffer, buff_size, fp);
    return buffer;
}

int getNumberOfQuestions(char* buffer, int buff_size, FILE* fp){
    memset(&buffer[0],0,buff_size);  
    fgets(buffer, buff_size, fp);
    //printf("INLINE: %s\n",buffer);
    return atoi(buffer);
}

void getQuestions(sQuestion* db, int qnum, char* buffer, int buff_size, FILE* fp){
    for(int i=0;i<qnum;++i){
        //db[i].text = readLine(buffer, buff_size, fp);
        strcpy(db[i].text, readLine(buffer, buff_size, fp));
        db[i].answer = atoi(readLine(buffer, buff_size, fp));
        db[i].used = 0; 
    }
}


#endif