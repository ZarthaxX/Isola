#ifndef TYPES_HPP
#define TYPES_HPP
#include <iostream>
#include <cstdint>

#include "constants.hpp"

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

typedef uint64_t Board;
typedef uint64_t Moves;
typedef uint8_t Move;
typedef uint64_t Removes;
typedef uint8_t Remove;
typedef uint16_t MoveRemove;
typedef uint8_t Player;
typedef uint64_t Key;
typedef uint8_t Depth;
typedef int32_t Value;

constexpr MoveRemove NULL_MOVE = 0;

typedef enum{
  BetaCutoff = 0,
  AlphaCutoff,
  Exact,
}Bound;


void debug_move(Move m){
    //cerr << "(" <<  from_sq(m)%MAP_WIDTH << "," << from_sq(m)/MAP_WIDTH << ") -> (" << to_sq(m)%MAP_WIDTH << "," << to_sq(m)/MAP_WIDTH << ")" <<  endl;
}

#endif