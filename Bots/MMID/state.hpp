#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <ostream>
#include <vector>

using namespace std;

struct Point{
    int8_t x;
    int8_t y;
    operator==(const Point& o){
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

enum Cell{
    TILE = 2,
    EMPTY
};

int boardW;
int boardH;

inline bool isPointValid(const Point& p){
    return 0 <= p.x && p.x < boardW &&
           0 <= p.y && p.y < boardH;
}

struct State{
    uint8_t board[MAX_HEIGHT][MAX_WIDTH];
    Point players[2];
    bool operator==(const State& st){
        for(int y=0;y<boardH;y++){
            for(int x=0;x<boardW;x++){
                if(board[y][x]!=st.board[y][x])
                    return false;
            }
        }
        return players[0] == st.players[0] && players[1] == st.players[1];
    }
};

inline bool canRemoveCell(const State& st, const Point& remove){
    return st.board[remove.y][remove.x] == Cell::TILE;
}

#endif