#include "board.h"

Point::Point(int _x,int _y){
    x=_x;
    y=_y;
}

bool Point::operator!=(const Point& o){
    return !(*this == o);
}

bool Point::operator==(const Point& o){
    return o.x == x && o.y == y;
}

Point& Point::operator+=(const Point& o){
    x+=o.x;
    y+=o.y;
    return *this;
}

const Point directions[8] = {
    Point(-1,0), //W
    Point(-1,1), //NW
    Point(0,1),  //N
    Point(1,1),  //NE
    Point(1,0),  //E
    Point(1,-1), //SE
    Point(0,-1), //S
    Point(-1,-1) //SW
};

Player::Player(int x, int y, int _id) : pos(x,y){
    id=_id;
}

Action::Action(Point& _to,Point& _remove) : to(_to), remove(_remove){

}

std::ostream& operator<<(std::ostream &out, const Action &a) {
    out << a.to.x << " " << a.to.y << " " << a.remove.x << " " << a.remove.y;
    return out;
}

Board::Board(int height, int width){
    grid = vector<vector<bool>>(height,vector<bool>(width,false));
}

void Board::addPlayer(Player player){
    players[player.id] = player;
}

void Board::setCell(int x, int y, bool value){
    grid[y][x] = value;
}


bool Board::isValidPos(Point& p){
    int height = grid.size();
    int width = grid[0].size();

    return 
        0<=p.y && p.y<height &&
        0<=p.x && p.x<width;
}

void Board::doAction(Action action, int playerId){
    players[playerId].pos = action.to;
    setCell(action.remove.x,action.remove.y, true);
}

vector<Action> Board::getPossibleActions(int playerId){
    
    int enemyId = 1 - playerId;
    Point player = players[playerId].pos;
    Point enemy = players[enemyId].pos;
    int height = grid.size();
    int width = grid[0].size();

    vector<Action>possibleActions;

    for(const Point& dir : directions){
        Point to(player);
        to.x+=dir.x;
        to.y+=dir.y;

        if( isValidPos(to) && 
            grid[to.y][to.x] != REMOVED &&
            to != enemy){

            for(int y=0;y<height;y++){
                for(int x=0;x<width;x++){
                    Point remove(x,y);
                    if( grid[remove.y][remove.x] != REMOVED && 
                        remove != enemy &&
                        remove != to){
                        possibleActions.push_back(Action(to,remove));
                    }
                }
            }
        }
    }

    return possibleActions;
}

std::ostream& operator<<(std::ostream &out, const Board &b) {
    Point player1 = b.players[0].pos;
    Point player2 = b.players[1].pos;
    int height = b.grid.size();
    int width = b.grid[0].size();

    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            Point cell(x,y);
            if(cell == player1){
                out<<"0";
            }else if(cell == player2){
                out<<"1";
            }else{
                if(b.grid[y][x] == REMOVED){
                    out<<"#";
                }else{
                    out<<".";
                }
            }
        }
        out<<endl;
    }
    return out;
}