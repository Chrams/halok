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



int checkAnswers(char* buffer){
    /*if(strstr(buffer,"0")!=NULL || strstr(buffer,"1")!=NULL || strstr(buffer,"2")!=NULL ||
       strstr(buffer,"3")!=NULL || strstr(buffer,"4")!=NULL || strstr(buffer,"5")!=NULL ||
       strstr(buffer,"6")!=NULL || strstr(buffer,"7")!=NULL || strstr(buffer,"8")!=NULL ||
       strstr(buffer,"9")!=NULL || strstr(buffer,"-")!=NULL || strstr(buffer,"-")!=NULL)*/
       if(buffer[0]=='0' || buffer[0]=='1' || buffer[0]=='2' || buffer[0]=='3' || buffer[0]=='4' ||
       buffer[0]=='5' || buffer[0]=='6' || buffer[0]=='7' || buffer[0]=='8' || buffer[0]=='9' || buffer[0]=='-' || buffer[0]=='-')
       return 1;
       else return 0;
    
}


#endif