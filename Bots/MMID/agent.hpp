#ifndef AGENT_HPP
#define AGENT_HPP

#include <limits.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <chrono>

#include "state.hpp"

using namespace std;
using namespace chrono;

//high_resolution_clock::time_point now = high_resolution_clock::now();
//#define TIME duration_cast<duration<double>>(high_resolution_clock::now() - now).count()
#define NOW high_resolution_clock::now()

#define TIMEOUT 40000 //10 ms timeout
#define TIMEOUT2 900000
#define Now() chrono::high_resolution_clock::now()

static struct Stopwatch {
	chrono::high_resolution_clock::time_point c_time, c_timeout;
	void setTimeout(int us) { c_timeout = c_time + chrono::microseconds(us); }
	void Start() { c_time = Now();}
	inline bool Timeout() { return Now() > c_timeout; }
	long long EllapsedMicroseconds() { return chrono::duration_cast<chrono::microseconds>(Now() - c_time).count(); }
	long long EllapsedMilliseconds() { return chrono::duration_cast<chrono::milliseconds>(Now() - c_time).count(); }
} stopwatch;//} Stopwatch

namespace Agent{
    struct Action{
        Point to;
        Point remove;
    };

    constexpr int INF = INT_MAX;
    constexpr int LOSS = -1000000;
    constexpr int WIN = -LOSS;

    int timeLimit;
    long long nodeCount;
    int depthLimit= 1;
    Action bestAction;

    int minmax(State& state, int8_t playerTurn, int8_t depth, int alpha, int beta){
        if(stopwatch.Timeout())
            throw exception();
        nodeCount++;
        if(depth == 0){
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

                            int score = -minmax(state,1 - playerTurn,depth-1, -beta, -alpha);
                            
                            if( score >= beta ){
                                state.board[remove.y][remove.x] = Cell::TILE;
                                state.board[player.y][player.x] = Cell::TILE;
                                state.board[playerCopy.y][playerCopy.x] = playerTurn;
                                player.x -= dir.x;
                                player.y -= dir.y;
                                return beta;   //  fail hard beta-cutoff
                            }
                        
                            if(score > alpha){
                                alpha = score;
                                if(depth == depthLimit){
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
            alpha = LOSS-depth;
        return alpha;
    }


    Action findAction(State& state, int8_t playerId){

        stopwatch.setTimeout((timeLimit-50)*1000);
        Action bestActionDepth;
        depthLimit= 1;
        while(!stopwatch.Timeout() && depthLimit <= (boardW+1)*(boardH+1)){
            try{
                nodeCount = 0;
                int score = minmax(state, playerId, depthLimit, -INF, INF);
                //cerr << "depth " << depthLimit << " | score " << score << " | nodeCount " << nodeCount << endl;
                bestActionDepth = bestAction;
                if(score==WIN)
                    break;
            }catch(exception e){
                cerr<<"Timeout at depth " << depthLimit << " | nodeCount " << nodeCount << endl;
            }
            depthLimit++;
        }
        return bestActionDepth;
    }
}


#endif