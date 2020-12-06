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
            return scores[playerTurn] - scores[1-playerTurn];
        }   

        while(hasMoves(moves)){
            Move move = getNextMove(moves);
            state = movePlayer(state,playerTurn,move);
            Removes removes = getAvailableRemoves(state.board, move, enemyCopy);

            while(hasRemoves(removes)){
                Remove remove = getNextRemove(removes);
                state = removeCell(state,remove);
                
                int score = -minmax(state,1 - playerTurn,depth-1, -beta, -alpha);
                
                if( score >= beta ){
                    state.board = boardCopy; 
                    state.players[playerTurn] = playerCopy;
                    return beta;   //  fail hard beta-cutoff
                }
                if(score > alpha){
                    alpha = score;
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
                //cerr << "depth " << depthLimit << " | score " << score << " | nodeCount " << nodeCount << endl;
                bestActionDepth = bestAction;
                if(score>=WIN || score <= LOSS)
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