#ifndef AGENT_HPP
#define AGENT_HPP

#include <limits.h>
#include <iostream>
#include <assert.h>

#include "state.hpp"

using namespace std;

namespace Agent{
    struct Action{
        Point to;
        Point remove;
    };

    int INF = INT_MAX;

    int DEPTH_LIMIT = 3;

    Action bestAction;

    int minmax(State& state, int8_t playerTurn, int8_t depth, int alpha, int beta){
        State stateCopy = state;
        if(depth == DEPTH_LIMIT){
            int scores[2]={0,0};
            for(int8_t p = 0;p < 2;p++){
                Point& player = state.players[p];

                for(const Point& dir : directions){
                    player.x += dir.x;
                    player.y += dir.y;

                    if(isPointValid(player) && state.board[player.y][player.x] == Cell::TILE){
                        scores[p]++;
                    }

                    player.x -= dir.x;
                    player.y -= dir.y;

                }
            }
            return scores[playerTurn] - scores[1-playerTurn];
        }   

        Point& player = state.players[playerTurn];
        Point playerCopy = state.players[playerTurn];
        
        bool noMoves=true;
        for(const Point& dir : directions){
            player.x += dir.x;
            player.y += dir.y;

            if(isPointValid(player) && state.board[player.y][player.x] == Cell::TILE){
                state.board[playerCopy.y][playerCopy.x] = Cell::TILE;
                state.board[player.y][player.x] = playerTurn;

                for(int8_t y = 0;y < boardH;y++){
                    for(int8_t x=0;x < boardW;x++){
                        Point remove = {x,y};
                        if(canRemoveCell(state,remove)){
                            noMoves = false;

                            state.board[remove.y][remove.x] = Cell::EMPTY;

                            int score = -minmax(state,1 - playerTurn,depth+1, -beta, -alpha);
                            
                            if( score >= beta ){
                                state.board[remove.y][remove.x] = Cell::TILE;
                                state.board[player.y][player.x] = Cell::TILE;
                                state.board[playerCopy.y][playerCopy.x] = playerTurn;
                                player.x -= dir.x;
                                player.y -= dir.y;
                                assert(state==stateCopy);
                                return beta;   //  fail hard beta-cutoff
                            }
                        
                            if(score > alpha){
                                alpha = score;
                                if(depth == 0){
                                    bestAction.to = player;
                                    bestAction.remove = remove;
                                }
                            }
                           
                            state.board[remove.y][remove.x] = Cell::TILE;
                        }
                    }
                }

                state.board[player.y][player.x] = Cell::TILE;
                state.board[playerCopy.y][playerCopy.x] = playerTurn;
            }

            player.x -= dir.x;
            player.y -= dir.y;
        }
        if(noMoves)
            alpha = -1000000;
        return alpha;
    }


    Action findAction(State& state, int8_t playerId){
        int score = minmax(state, playerId, 0, -INF, INF);
        cerr << score << endl;
        return bestAction;
    }
}


#endif