It's a Chess AI written in C. The chess engine uses [MiniMax](https://en.wikipedia.org/wiki/Minimax) algorithm with [Alpha-Beta](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) pruning to play the moves. It walks down the game-tree 3 levels deep fast enough. With 4, it might take long seconds to play. The evaluation function only considers the army strength and their positions. 
 
The game follows the standard chess rules; can handle the special moves of castling, en-passant, promotions, etc. It does not handle threefold repetition, though.
<br><br><br><br>
![](http://i.imgur.com/s9azOYO.gif)

# Getting started

## Prerequisites
A C compiler is all you need.

## Installation
Compile and run the program. The game is on!

## How to play
Please refer to the gif above for the input/output format.

Enter the cell positions as <*file*><*rank*> notations, e.g. **a1**, **b5**, **h8**, etc., where the letters *a* through *h* represent the file(column) number and the numbers *1* through *8* represent the rank(row) number.<br><br>The chessboard has a fixed arrangement of the black set playing from the bottom and white from the top.<br><br>The interface will keep you informed about the chess pieces being moved, the captures, etc. ( and would obnoxiously keep asking you to quit every time its your turn! :stuck_out_tongue: )
<br><br><br><br>
*Note*: I wrote this ages ago, in my first year of BE; I'm aware there might be some unattended bugs lurking in there, if you find one, please report!
