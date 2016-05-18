#ifndef _GAME_H
#define _GAME_H

#include "player.h"

//---------------------------
//functions
//---------------------------
int matchMaking(sPlayer* player, int answer){
    int dev1, dev2;
    dev1 = abs(player[0].answer-answer);
    dev2 = abs(player[1].answer-answer);
    if(dev1<dev2)
        return 0;
    else if(dev2<dev1)
        return 1;
    else return -1;
}

#endif