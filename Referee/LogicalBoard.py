import copy

from Point import *

TILE = 0
NO_TILE = 1


directions = [    
    Point(-1,0), #W
    Point(-1,1), #NW
    Point(0,1),  #N
    Point(1,1),  #NE
    Point(1,0),  #E
    Point(1,-1), #SE
    Point(0,-1), #S
    Point(-1,-1) #SW
]

class Action:
    def __init__(self,to,remove):
        self.to = to
        self.remove = remove

    def __str__(self):
        return "[ to: " + str(self.to) + " | remove: " + str(self.remove) + " ]"

class LogicalBoard:
    def __init__(self,height,width):
        self.height = height
        self.width = width
        self.board = [[TILE for x in range(width)] for y in range(height)]
        self.players = [Point((width-1)//2,0),Point(width//2,height-1)]
        self.playerTurn = 0

    def copy(self, logicalBoard):
        self.height = logicalBoard.height
        self.width = logicalBoard.width
        self.board = copy.deepcopy(logicalBoard.board)
        self.players = copy.deepcopy(logicalBoard.players)
        self.playerTurn = logicalBoard.playerTurn

    def doAction(self,action):
        assert(self.isValidAction(action))

        self.doMove(action.to)
        self.doRemove(action.remove)

    def doMove(self,to):
        self.players[self.playerTurn] = to

    def doRemove(self,remove):
        self.board[remove.y][remove.x] = NO_TILE
        self.playerTurn = 1 - self.playerTurn

    def startGame(self,first):
        self.playerTurn = first
        self.board = [[TILE for x in range(self.width)] for y in range(self.height)]
        self.players = [Point((self.width-1)//2,0),Point(self.width//2,self.height-1)]

    def isGameDone(self):
        return 0 == len(self.getAvailableMoves(self.playerTurn))

    def getWinner(self):
        return (1 - self.playerTurn)

    def getAvailableMoves(self, player):
        availableMoves = []
        actualPlayer = self.players[player]

        for dir in directions:
            if self.isValidMove(player, dir+actualPlayer):
                availableMoves.append(dir+actualPlayer)

        return availableMoves

    def isValidMove(self, player, to):
        enemy = self.players[1-player]

        if self.isValidPos(to):
            return not (enemy == to) and self.board[to.y][to.x] == TILE
        else:
            return False

    def isValidAction(self, action):
        return self.isValidMove(self.playerTurn,action.to) and self.isValidRemove(self.playerTurn,action.to,action.remove)
            

    def isValidRemove(self, player, to, remove):

        enemy = self.players[1-player]
        
        return self.isValidPos(remove) and remove != to and remove != enemy and self.board[remove.y][remove.x] == TILE

    def playerToString(self,player):
        return f'{self.players[player].x} {self.players[player].y} {player}'

    def boardToString(self):
        cellToStr = { 0 : '.',
                      1 : '#'
                    }
        stringBoard = ''.join(
                    [(''.join(
                        [str(self.players.index(Point(x,y))) if Point(x,y) in self.players else cellToStr[self.board[y][x]] for x in range(self.width)])+'\n') for y in range(self.height)]
                )

        return stringBoard

    def isValidPos(self,pos):
        return 0 <= pos.x < self.width and 0 <= pos.y < self.height