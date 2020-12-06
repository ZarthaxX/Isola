#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <time.h>

#include "board.h"

using namespace std;

int main(){

    srand(time(NULL));

    int games;
    cin>>games;

    int myPlayer;
    cin>>myPlayer;

    int height,width;
    cin>>height>>width;

    int timeLimit;
    cin>>timeLimit;

    while(games-- > 0){

        while(true){

            int gameDone;
            cin>>gameDone;
            
            if(gameDone != -1)
                break;
            
            Board board(height,width);
            
            for(int y=0;y<height;y++){
                for(int x=0;x<width;x++){
                    char cell;
                    cin>>cell;
                    if(cell=='#')
                        board.setCell(x,y,REMOVED);
                    if(cell=='0')
                        board.addPlayer(Player(x,y,0));
                    if(cell=='1')
                        board.addPlayer(Player(x,y,1));
                }
            }
            
            vector<Action> allActions = board.getPossibleActions(myPlayer);
            random_shuffle(allActions.begin(),allActions.end());
            Action bestAction;
            int bestAmount = -10000;
            for(Action a : allActions){
                auto boardCopy = board;
                boardCopy.doAction(a,myPlayer);
                int amountActions = boardCopy.getPossibleActions(myPlayer).size() - boardCopy.getPossibleActions(1-myPlayer).size();
                if(amountActions > bestAmount){
                    bestAmount = amountActions;
                    bestAction = a;
                }
            }
            //bestAction = allActions[0];
            cout << bestAction << endl;
        }
    }

    return 0;
}