import argparse
from Constants import *
from Referee import *

def main(args):
    referee = Referee(args.player1,args.player2,args.height,args.width,args.timelimit)

    referee.playGame(args.rounds,args.first-1)

if __name__== "__main__":
    description = 'Isola board game by ZarthaxX'
    parser = argparse.ArgumentParser(description=description)

    parser.add_argument('--player1',
                        type=str,
                        default=None,
                        help='Path for player1\'s bot executable.')
    parser.add_argument('--player2',
                        type=str,
                        default=None,
                        help='Path for player1\'s bot executable.')
    parser.add_argument('--first',
                        type=int,
                        choices=[1,2],
                        default=1,
                        help='Player that goes first.')
    parser.add_argument('--timelimit',
                        type=int,
                        default=1000,
                        help='Time in ms allowed per turn for a bot.')
    parser.add_argument('--rounds',
                        type=int,
                        default=1,
                        help='Number of rounds for the game.')
    parser.add_argument('--height',
                        type=int,
                        choices=[2,3,4,5,6,7,8],
                        default=7,
                        help='Height of the board.')
    parser.add_argument('--width',
                        type=int,
                        choices=[2,3,4,5,6,7,8],
                        default=7,
                        help='Width of the board.')

    args = parser.parse_args()

    main(args)

