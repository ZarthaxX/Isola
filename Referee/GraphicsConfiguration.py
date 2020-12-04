
from Point import *

class Resources:

    player0 = "Resources/Tokens/1.png"
    player1 = "Resources/Tokens/2.png"
 
    emptyCell = "Resources/Tiles/empty.png"
    tileCell = "Resources/Tiles/tile.png"
    
    moveableCell = "Resources/Tiles/moveableCell.png"
    selectedToken = "Resources/Tiles/selectedPlayer.png"

    tokenSize = Point(70,70)

BOARD_WIDTH_OFFSET = Resources.tokenSize.x
BOARD_HEIGHT_OFFSET = Resources.tokenSize.y

NORMAL_FPS = 60
BOT_FPS = 10