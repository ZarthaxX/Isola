#ifndef AGENT_HPP
#define AGENT_HPP

#include <limits.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <string>
#include <stdio.h>

#include "state.hpp"
#include "globals.hpp"
#include "stopwatch.hpp"

using namespace std;

namespace Agent{
    struct Action{
        Point to;
        Point remove;
    };

    int depthLimit;
    long long nodeCount;
    Action bestAction;
    
    int moveScore[MAX_HEIGHT*MAX_WIDTH];

    int sortMoves(Board board,Player enemy,Moves moves, Move sortedMoves[8]){
        int idx=0;
        while(hasMoves(moves)){
            Move move = getNextMove(moves);
            sortedMoves[idx++] = move;
            moveScore[move] = getMoveCount(board,move,enemy) - getMoveCount(board,enemy,move);
        }
        sort(sortedMoves,sortedMoves+idx-1,[](Move m1,Move m2){return moveScore[m1]>moveScore[m2];});
        return idx;
    }

    int sortRemoves(Board board,Player enemy,Removes removes, Move sortedRemoves[64]){
        int idx=0;
        int enemyX = enemy&(MAX_HEIGHT-1);
        int enemyY = enemy>>3;
        while(hasRemoves(removes)){
            Remove remove = getNextRemove(removes);
            sortedRemoves[idx++] = remove;
            moveScore[remove] = abs(enemyX-(remove&(MAX_HEIGHT-1)))+abs(enemyY-(remove>>3));
        }
        sort(sortedRemoves,sortedRemoves+idx-1,[](Remove r1,Remove r2){return moveScore[r1]<moveScore[r2];});
        return idx;
    }

    int minmax(State& state, int8_t playerTurn, int8_t depth, int alpha, int beta){
        if(stopwatch.Timeout())
            throw exception();
        nodeCount++;
        
        uint8_t found;

        Board boardCopy = state.board;
        Player playerCopy = state.players[playerTurn];
        Player enemyCopy = state.players[1-playerTurn];
        Moves moves = getAvailableMoves(boardCopy, playerCopy, enemyCopy);
        if(!hasMoves(moves)){
            return LOSS-depth;
        }
 
        if(depth == 0){
            int scores[2]={0,0};
            for(int8_t p = 0;p < 2;p++){
                //scores[p] = getMoveCount(state.board, state.players[p], state.players[1-p]);
                Moves futureMoves = getAvailableMoves(state.board, state.players[p], state.players[1-p]);
                Player playerCopy = state.players[p];
                while(hasMoves(futureMoves)){
                    Move move = getNextMove(futureMoves);
                    state = movePlayer(state,p,move);
                    scores[p] += getMoveCount(state.board, state.players[p], state.players[1-p]);
                    state.players[p] = playerCopy;
                }
            }
            
            int score = scores[playerTurn] - scores[1-playerTurn];

            return score;
        }
           
        MoveRemove bestMoveRemove = NULL_MOVE;
        Move sortedMoves[8];
        int moveCount = sortMoves(boardCopy,enemyCopy,moves,sortedMoves);
        int moveIndex = 0;
        while(moveIndex < moveCount){
            Move move = sortedMoves[moveIndex++];
            state = movePlayer(state,playerTurn,move);
            Removes removes = getAvailableRemoves(state.board, move, enemyCopy);
            Remove sortedRemoves[64];
            int removeCount = sortRemoves(boardCopy,enemyCopy,removes,sortedRemoves);
            int removeIndex = 0;
            while(removeIndex < removeCount){
                Remove remove = sortedRemoves[removeIndex++];
                state = removeCell(state,remove);
                
                int score = -minmax(state,1-playerTurn,depth-1,-beta,-alpha);
              
                if( score >= beta ){
                    
                    state.board = boardCopy; 
                    state.players[playerTurn] = playerCopy;

                    return beta;   //  fail hard beta-cutoff
                }
                if(score > alpha){
                    alpha = score;
                    bestMoveRemove = joinMoveRemove(move,remove);
                    if(depth == depthLimit){
                        bestAction.to = {move%MAX_HEIGHT,move/MAX_HEIGHT};
                        bestAction.remove = {remove%MAX_HEIGHT,remove/MAX_HEIGHT};
                    }
                }
                state.board = boardCopy; 
            }
            state.players[playerTurn] = playerCopy;
        }
    
        return alpha;
    }


    Action findAction(State& state, int8_t playerId){
        stopwatch.setTimeout((timeLimit-50)*1000);

        Action bestActionDepth;
        depthLimit= 1;
        while(!stopwatch.Timeout() && depthLimit <= boardW*boardH+1){
            try{
                nodeCount = 0;
                int score = minmax(state, playerId, depthLimit, -INF, INF);
                //cerr << "depth " << depthLimit << " | score " << score << " | nodeCount " << nodeCount << " | ttPrunes " << ttPrunes << endl;
                bestActionDepth = bestAction;
                if(score>=WIN || score <= LOSS)
                    break;
            }catch(exception e){
                cerr<<"Monster timeout at depth " << depthLimit << " | nodeCount " << nodeCount << endl;
            }
            depthLimit++;
        }
        return bestActionDepth;
    }
}


#endif


