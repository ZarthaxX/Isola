#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdio.h> 
#include <iostream>

#include "types.hpp"
#include "globals.hpp"

using namespace std;

struct State{
    Board board;
    Player players[2];
    bool operator==(const State& o) const{return board==o.board && players[0]==o.players[0] && players[1]==o.players[1];}
    bool operator!=(const State& o) const{return !(*this==o);}
};

Key getStateKey(const State& st){
    uint64_t hash =  
        st.board ^ 
        Zobrist::playerKey[0][st.players[0]] ^ 
        Zobrist::playerKey[1][st.players[1]];
    return ((hash >> 16*0) ^
           (hash >> 16*1) ^
           (hash >> 16*2) ^
           (hash >> 16*3)) & 0xFFFF;
}

void printState(State st){
    cerr << "Board"<<endl;
    for(int y=0;y < MAX_HEIGHT;y++){
        for(int x=0;x<MAX_WIDTH;x++){
            uint64_t cell = 1ULL & (st.board >> (y*MAX_HEIGHT+x));
            if(cell==0)cerr<<"#";
            if(cell==1)cerr<<".";
        }cerr<<endl;
    }
    cerr <<"Player 0: " << st.players[0]%MAX_HEIGHT << " " << st.players[0]/MAX_HEIGHT << endl;
    cerr <<"Player 1: " << st.players[1]%MAX_HEIGHT << " " << st.players[1]/MAX_HEIGHT << endl;
}

inline Board setCell(Board board,int x,int y){
    return board | (1ULL << (y*MAX_HEIGHT+x));
}

inline Board clrCell(Board board,int x,int y){
    return board & ~(1ULL << (y*MAX_HEIGHT+x));
}

inline Player toPlayer(int x,int y){
    return y*MAX_HEIGHT+x;
}

inline Moves getAvailableMoves(Board board, Player player, Player enemy){
    Moves moves = movesArray[player];
    moves &= board;
    moves &= ~(1ULL << enemy);
    return moves;
}

inline int getMoveCount(Board board, Player player, Player enemy){
    return __builtin_popcountll(getAvailableMoves(board,player,enemy));
}

inline bool hasMoves(Moves moves){
    return moves != 0ULL;
}

inline Move getNextMove(Moves& moves){
    Move move = __builtin_ffsll(moves)-1;
    moves &= ~(1ULL << move);
    return move;
}

inline Removes getAvailableRemoves(Board board, Player player, Player enemy){
    Removes removes = removesArray[enemy];
    removes &= board;
    removes &= ~(1ULL << player);
    removes &= ~(1ULL << enemy);
    if(removes==0){
        removes = board;
        removes &= ~(1ULL << player);
        removes &= ~(1ULL << enemy);
    }
    return removes;
}

inline int getRemoveCount(Board board, Player player, Player enemy){
    return __builtin_popcountll(getAvailableRemoves(board,player,enemy));
}

inline bool hasRemoves(Removes removes){
    return removes != 0ULL;
}

inline Remove getNextRemove(Removes& removes){
    Remove remove = __builtin_ffsll(removes)-1;
    removes &= ~(1ULL << remove);
    return remove;
}

inline State& movePlayer(State& state, int playerTurn, Move move){
    state.players[playerTurn] = move;
    return state;
}

inline State& removeCell(State& state, Remove remove){
    state.board &= ~(1ULL << remove);
    return state;
}

inline MoveRemove joinMoveRemove(Move move,Remove remove){
    return move | (remove << 8);
}

void initMovesArray(){
    for(int y=0;y<MAX_HEIGHT;y++){
        for(int x=0;x<MAX_WIDTH;x++){
            Board move = 0;
            for(auto dir : directions){
                Point ady = {x+dir.x,y+dir.y};
                if( 0<=ady.x&&ady.x<MAX_WIDTH&&
                    0<=ady.y&&ady.y<MAX_HEIGHT){
                    int idx=ady.y*MAX_HEIGHT+ady.x;
                    move |= (1ULL << idx);
                }
            }
            movesArray[y*MAX_HEIGHT+x] = move;
        }
    }
}

void initRemovesArray(){
    for(int y=0;y<MAX_HEIGHT;y++){
        for(int x=0;x<MAX_WIDTH;x++){
            Removes removes = 0;
            for(int y2=0;y2<MAX_HEIGHT;y2++){
            	for(int x2=0;x2<MAX_WIDTH;x2++){
            		int dy=abs(y-y2);
            		int dx=abs(x-x2);
            		if(dy+dx<=2){
            			removes |= 1ULL << (y2*MAX_HEIGHT+x2);
					}
				}
			}
            removesArray[y*MAX_HEIGHT+x] = removes;
        }
    }
}

#endif
