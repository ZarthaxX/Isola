#pragma GCC optimize "Ofast,omit-frame-pointer,inline,unroll-loops"
#pragma GCC option("arch=native", "tune=native", "no-zero-upper")

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <assert.h>

#include "state.hpp"
#include "agent.hpp"
#include "globals.hpp"

using namespace std;

int main(){

    srand(time(NULL));

    int games;
    cin>>games;

    int myPlayer;
    cin>>myPlayer;

    cin>>boardH>>boardW;

    cin>>timeLimit;

    initMovesArray();
    initRemovesArray();

    while(games-- > 0){

        while(true){

            int gameDone;
            cin>>gameDone;
            
            if(gameDone != -1)
                break;
            
            State state;
            state.board = 0;
            for(int8_t y=0;y<boardH;y++){
                for(int8_t x=0;x<boardW;x++){
                    char cell;
                    cin>>cell;
                    if(cell!='#')
                        state.board = setCell(state.board,x,y);
                    if(cell=='#')
                        state.board = clrCell(state.board,x,y);
                    if(cell=='0')
                        state.players[0] = toPlayer(x,y);
                    if(cell=='1')
                        state.players[1] = toPlayer(x,y);
                }
            }
            stopwatch.Start();
            Agent::Action action = Agent::findAction(state,myPlayer);

            cout << (int)action.to.x << " " << (int)action.to.y << " " << (int)action.remove.x << " " << (int)action.remove.y << endl;
        }
    }

    return 0;
}

/*
1
0
7
7
1000
-1
...0...
.......
.......
.......
.......
.......
...1...
*/