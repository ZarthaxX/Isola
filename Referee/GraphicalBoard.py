import sys
import pygame
import copy

from LogicalBoard import *
from GraphicsConfiguration import *
from Point import *
from Constants import *

       
ALPHA = (249,19,191)
WHITE = (255, 255, 255)
BLUE= (63,72,204)
RED= (237,28,36)
BROWN = (200,145,44)

class GraphicalBoard:   
    entityToImg = {
        '0' : "player0",
        '1' : "player1",
        '.' : "tile",
        '#' : "empty"
    }

    def __init__(self, logicalBoard : LogicalBoard, playerScores):
        self.logicalBoard = logicalBoard
        self.playerScores = playerScores
        pygame.init()
        pygame.display.set_caption("Isola")
        programIcon = pygame.image.load("Resources\icon.bmp")
        pygame.display.set_icon(programIcon)
        self.boardLocation = Point(BOARD_WIDTH_OFFSET,BOARD_HEIGHT_OFFSET)
        self.span = Point(2,2)
        self.screenSize = self.toVisualPoint(Point(self.logicalBoard.width+1,self.logicalBoard.height+1))
        self.screen = pygame.display.set_mode(self.screenSize)
        self.clock = pygame.time.Clock()
        pygame.font.init()
        self.font = pygame.font.SysFont('Comic Sans MS', 30)

        self.selectedToken = pygame.image.load(Resources.selectedToken).convert()
        self.selectedToken.set_colorkey(ALPHA)
        self.moveableCell = pygame.image.load(Resources.moveableCell).convert()
        self.moveableCell.set_colorkey(ALPHA)

        self.cells = {}
        self.cells[1] = pygame.image.load(Resources.emptyCell).convert()
        self.cells[1].set_colorkey(ALPHA)
        self.cells[0] = pygame.image.load(Resources.tileCell).convert()
        self.cells[0].set_colorkey(ALPHA)
        self.players = {}
        self.players[Player.PLAYER0] = pygame.image.load(Resources.player0).convert()
        self.players[Player.PLAYER0].set_colorkey(ALPHA)
        self.players[Player.PLAYER1] = pygame.image.load(Resources.player1).convert()
        self.players[Player.PLAYER1].set_colorkey(ALPHA)
        

        self.FPS = NORMAL_FPS

        self.terminated = False
        
    def draw(self,availableMoves : list() = list()) -> None:
        
        self.screen.fill(WHITE)
        #pygame.draw.rect(self.screen,WHITE,(self.board_loc[0],self.board_loc[1],2000,2000))
        #self.screen.blit(self.board_image,self.board_loc)
        pygame.draw.rect(
            self.screen,BROWN,
            (self.boardLocation.x,
             self.boardLocation.y,
             self.logicalBoard.width*(Resources.tokenSize.x + self.span.x) + self.span.x,
             self.logicalBoard.height*(Resources.tokenSize.y + self.span.y) + self.span.y))


        for y in range(self.logicalBoard.height):
            for x in range(self.logicalBoard.width):
                cell = self.logicalBoard.board[y][x]
                point = self.toVisualPoint(Point(x,y))
                self.screen.blit(self.cells[cell], point)

        for i in range(2):
            player = self.logicalBoard.players[i]
            point = self.toVisualPoint(player)
            self.screen.blit(self.players[i],point)
        
        #point = self.toVisualPoint(self.logicalBoard.players[playerTurn])
        #self.screen.blit(self.selectedToken,point)
            
        #for move in availableMoves:
        #    point = self.toVisualPoint(move)
        #    self.screen.blit(self.moveableCell,point)
    
        playerTurn = "red" if self.logicalBoard.playerTurn == 0 else "blue"
        playerTurnColor = RED if self.logicalBoard.playerTurn == 0 else BLUE
        playerTurnText = self.font.render(f"Player {playerTurn}'s turn", False, playerTurnColor)
        player1ScoreText = self.font.render(f"W {self.playerScores[0]}", False, RED)
        player2ScoreText = self.font.render(f"W {self.playerScores[1]}", False, BLUE)
        player1Location = self.toVisualPoint(Point(-1,(self.logicalBoard.height-1)//2))
        player2Location = self.toVisualPoint(Point(self.logicalBoard.width,(self.logicalBoard.height-1)//2))
        player1TextLocation = self.toVisualPoint(Point(-1,(self.logicalBoard.height-1)//2+1))
        player2TextLocation = self.toVisualPoint(Point(self.logicalBoard.width,(self.logicalBoard.height-1)//2+1))
        self.screen.blit(self.players[0],player1Location)
        self.screen.blit(self.players[1],player2Location)
        self.screen.blit(player1ScoreText,(player1TextLocation[0]+(Resources.tokenSize.x-player1ScoreText.get_rect().width)//2,player1TextLocation[1]))
        self.screen.blit(player2ScoreText,(player2TextLocation[0]+(Resources.tokenSize.x-player2ScoreText.get_rect().width)//2,player2TextLocation[1]))
        boardWidth = self.toVisualPoint(Point(self.logicalBoard.width,self.logicalBoard.height))[0] - self.toVisualPoint(Point(0,0))[0]
        playerTurnLocation = (self.toVisualPoint(Point(0,0))[0] + (boardWidth - playerTurnText.get_rect().width) // 2,
                              (self.boardLocation.y - playerTurnText.get_rect().height)//2)
        self.screen.blit(playerTurnText,playerTurnLocation)
        pygame.display.update()
        self.clock.tick(self.FPS)

        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONUP:
                None   
    
    def animateAction(self, action):
  
        self.FPS = BOT_FPS
        logicalBoard = copy.deepcopy(self.logicalBoard)
        self.logicalBoard.doMove(action.to)
        self.draw()
        self.logicalBoard.doRemove(action.remove)
        self.draw()
        self.logicalBoard.copy(logicalBoard)

    def toVisualPoint(self,p : Point) -> (int,int):
    
        point = copy.copy(p)
        point.x = (Resources.tokenSize.x + self.span.x) * point.x + self.span.x + self.boardLocation.x
        point.y = (Resources.tokenSize.y + self.span.y) * point.y + self.span.y + self.boardLocation.y
      
        return (point.x,point.y)

    def toBoardPoint(self,p : (int,int)) -> Point:
    
        point = Point(p[0],p[1])
        point.x = (point.x - self.span.x - self.boardLocation.x) // (Resources.tokenSize.x + self.span.x) 
        point.y = (point.y - self.span.y - self.boardLocation.y) // (Resources.tokenSize.y + self.span.y)
      
        return point


    def exit(self):
        pygame.quit()
        self.terminated = True