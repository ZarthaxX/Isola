#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <ostream>
#include <vector>
#include <stdio.h> 
#include <iostream>

using namespace std;

struct Point{
    int x;
    int y;
    bool operator==(const Point& o){
        return x==o.x&&y==o.y;
    }
};

constexpr Point directions[8] = {
    {0,-1}, //N  ↑  
    {1,-1}, //NE ↗
    {1,0},  //E  →
    {1,1},  //SE ↘
    {0,1},  //S  ↓
    {-1,1}, //SW ↙
    {-1,0}, //W  ←
    {-1,-1} //NW ↖
};

constexpr int MAX_WIDTH = 8;
constexpr int MAX_HEIGHT = 8;

int boardW;
int boardH;

typedef uint64_t Board;
typedef uint64_t Moves;
typedef uint8_t Move;
typedef uint64_t Removes;
typedef uint8_t Remove;
typedef uint8_t Player;

Moves movesArray[MAX_HEIGHT*MAX_WIDTH];

struct State{
    Board board;
    Player players[2];
};

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
    Removes removes = board;
    removes &= ~(1ULL << player);
    removes &= ~(1ULL << enemy);
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



#endif