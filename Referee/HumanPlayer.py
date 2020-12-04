from GraphicalBoard import *
import copy

class HumanPlayer:
    def __init__(self,playerId : str,graphicalBoard):
        self.human = False
        self.id = playerId
        self.graphicalBoard = graphicalBoard

    def send(self, msg):
        pass
    def receive(self, cast):
        action = self.getHumanMove()
        return action

    def finish(self):
        pass

    def getHumanMove(self):

        self.FPS = NORMAL_FPS

        graphicalBoard = self.graphicalBoard
        logicalBoardCopy = copy.deepcopy(graphicalBoard.logicalBoard)

        actionTo = None
        actionRemove = None
        done = False

        self.graphicalBoard.draw()
        actionTo = self.movingToken()

        if self.graphicalBoard.terminated: 
            return None

        graphicalBoard.logicalBoard.doMove(actionTo)
        graphicalBoard.draw()

        actionRemove = self.removingCell(actionTo)

        if self.graphicalBoard.terminated: 
            return None

        graphicalBoard.logicalBoard.copy(logicalBoardCopy)
        graphicalBoard.draw()

        action = Action(actionTo,actionRemove)
        return action

    def decodeMouse(self) -> Point:
        return self.graphicalBoard.toBoardPoint(pygame.mouse.get_pos())
    
    def movingToken(self) -> Point:
        
        graphicalBoard = self.graphicalBoard
        logicalBoard = graphicalBoard.logicalBoard

        to = Point(-1,-1)

        while not graphicalBoard.terminated and to.x == -1 and to.y == -1:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    graphicalBoard.exit()
                    return to
                elif event.type == pygame.MOUSEBUTTONUP:
                    selected = self.decodeMouse()
                    if selected in logicalBoard.getAvailableMoves(logicalBoard.playerTurn):
                        to = selected
                        break

        return to

    def removingCell(self, to):
 
        graphicalBoard = self.graphicalBoard
        logicalBoard = graphicalBoard.logicalBoard

        remove = Point(-1,-1)

        while not graphicalBoard.terminated and remove.x == -1 and remove.y == -1:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    graphicalBoard.exit()
                    return remove
                elif event.type == pygame.MOUSEBUTTONUP:
                    selected = self.decodeMouse()

                    if logicalBoard.isValidRemove(logicalBoard.playerTurn,to,selected):
                        remove = selected
                        break

                    graphicalBoard.draw()

        return remove

