#pragma once

#include <string>
#include <ostream>
#include <vector>

using namespace std;

class Point{
    public:
        Point(){};
        Point(int _x,int _y);
        bool operator!=(const Point& o);
        bool operator==(const Point& o);
        Point& operator+=(const Point& o);
        int x,y;
};

class Player{
    public:
        Player(){};
        Player(int x, int y, int _id);
        Point pos;
        int id;
};

class Action{
    public:
        Action(){};
        Action(Point& to,Point& remove);
        Point to;
        Point remove;

    friend std::ostream& operator<<(std::ostream &out, const Action &a);
};

const bool REMOVED = 1;

class Board{
    public:
        Board(int height, int width);
        void addPlayer(Player player);
        void setCell(int x, int y, bool value);
        vector<Action> getPossibleActions(int playerId);
        void doAction(Action action, int playerId);
	    friend std::ostream& operator<<(std::ostream &out, const Board &b);
    private:
        bool isValidPos(Point& p);
        Player players[2];
        vector<vector<bool>>grid;
};
