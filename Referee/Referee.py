from LogicalBoard import *
from GraphicalBoard import *
from HumanPlayer import *
from BotPlayer import *

PLAYER1 = 0
PLAYER2 = 1

class Referee:
    def __init__(self,player1, player2, boardHeight, boardWidth, timeLimit):
        self.timeLimit = timeLimit
        self.logicalBoard = LogicalBoard(boardHeight,boardWidth) 
        self.playerScores = [0,0]
        self.graphicalBoard = GraphicalBoard(self.logicalBoard, self.playerScores)

        if player1 is None:
            player1Controller = HumanPlayer(PLAYER1, self.graphicalBoard)
        else:
            player1Controller = BotPlayer(player1,PLAYER1,timeLimit)

        if player2 is None:
            player2Controller = HumanPlayer(PLAYER2, self.graphicalBoard)
        else:
            player2Controller = BotPlayer(player2,PLAYER2,timeLimit)

        self.players = [player1Controller,player2Controller]

    def playGame(self,games, first):
        
        for player in self.players:
            player.send(games)
            player.send(player.id)
            player.send(self.logicalBoard.height)
            player.send(self.logicalBoard.width)
            player.send(self.timeLimit)

        for round in range(games):
            winner = self.playRound(round,first)

            if self.graphicalBoard.terminated == True:
                break

            self.playerScores[winner]+=1
            first = self.nextRoundPlayer(first)
            

        print("Final score:")
        print("Player 0: ",self.playerScores[0])
        print("Player 1: ",self.playerScores[1])

    def playRound(self,round,first):
        
        self.logicalBoard.startGame(first)
        self.graphicalBoard.draw()

        winner = -1
        while not self.logicalBoard.isGameDone():
            playerTurn = self.logicalBoard.playerTurn
            #print("Player turn",playerTurn)
            currentPlayer = self.players[playerTurn]
            currentPlayer.send(-1)

            stringBoard = self.logicalBoard.boardToString()
            currentPlayer.send(stringBoard)
            
            #for player in self.players:
            #    currentPlayer.send(self.logicalBoard.playerToString(player.id))

            response = currentPlayer.receive(self.formatAction)
            
            if self.graphicalBoard.terminated == True:
                self.players[0].finish()
                self.players[1].finish()
                break

            if not self.logicalBoard.isValidAction(response):
                print(f"Invalid action for player {playerTurn}")
                winner = self.nextRoundPlayer(playerTurn)
                break
            
            self.graphicalBoard.animateAction(response)
            self.logicalBoard.doAction(response)
      
        if self.logicalBoard.isGameDone():
            winner = self.logicalBoard.getWinner()

        playerTurn = self.logicalBoard.playerTurn

        self.players[playerTurn].send(winner)
        self.players[self.nextRoundPlayer(playerTurn)].send(winner)

        return winner

    def nextRoundPlayer(self,first):
        if first == PLAYER1:
            return PLAYER2
        else:
            return PLAYER1

    
    def formatAction(self,action):
        data = action.split(' ')

        if len(data) != 4:
            return Action(Point(-1,-1),Point(-1,-1))
        
        toX = int(data[0])
        toY = int(data[1])
        removeX = int(data[2])
        removeY = int(data[3])

        return Action(Point(toX, toY), Point(removeX, removeY))