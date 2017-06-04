/**/
//A game of Tic-Tac-Toe between human and computer.
//Computer builds and explores the game trees using MiniMax rule.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define sym(t) (t==0)?'X':(t==1)?'O':' '
#define action(t) (t==0)?"crosses":"zeroes"

void humanmove(int board[3][3], int);
void computermove(int board[3][3], int);
int gameStatus(int board[3][3]);
int minmax(int board[3][3], int);

int draw=1;//Flag to indicate if the game is bound to result in a draw.

int main()
{
    int p, board[3][3]={-1,-1,-1,-1,-1,-1,-1,-1,-1}, winner, i, j;
    char q;
    
    printf("Cross or Circle? (x/o): ");
    do{
        scanf(" %c", &q);
    }while(q!='x'&&q!='X'&&q!='o'&&q!='O');
    
    if(q=='x'||q=='X') p=0;
    else if(q=='o'||q=='O') p=1;
    
    int turn=0;
    while(1)
    {
        //Drawing the current state of the game board
        printf("\n%c|%c|%c\n------\n", sym(board[0][0]), sym(board[0][1]), sym(board[0][2]));
        printf("%c|%c|%c\n------\n", sym(board[1][0]), sym(board[1][1]), sym(board[1][2]));
        printf("%c|%c|%c\n\n", sym(board[2][0]), sym(board[2][1]), sym(board[2][2]));
        
        winner = gameStatus(board);
        if(winner!=-1)
        {
            if(winner==p)
                printf("Congratulations! You Win!\n");
            else if(winner==2)
                printf("Draw!\n");
            else
                printf("Computer won!\n");
            break;
        }
        if(turn==p)//if player's turn
            humanmove(board, p);
        else
            computermove(board, (p+1)%2);
        turn=(turn+1)%2;
        
    }
   
    return 0;
}

//prompts player to input their next move, validates the move, and updates the board.
void humanmove(int board[3][3], int p)
{
    int cell, row, col;
    printf("Enter %c's cell number [1,9]: ", sym(p));
    start:scanf("%d", &cell);
    row=(cell-1)/3;
    col=(cell-1)%3;
    
    if(row<0||row>2||col<0||col>2)
    {   printf("Please enter a number in the range [1,9]: ");
        goto start;
    }
    
    if(board[row][col]!=-1)
    {
        printf("The entered cell is already occupied! Enter another cell number: ");
        goto start;
    }
    
    board[row][col]=p;
}

//Checks for wins/draws if the game's complete.
int gameStatus(int board[3][3])
{
    int i, j, x1=0, x2=0, o1=0, o2=0, xd1=0, xd2=0, od1=0, od2=0, win[2]={0,0};
    int board_full=1;
    
    for(i=0;i<3;i++)
    {   
        if(board[i][i]==0) xd1++;
        if(board[i][2-i]==0) xd2++;
        if(board[i][i]==1) od1++;
        if(board[i][2-i]==1) od2++;
        
        x1=x2=o1=o2=0;
        for(j=0;j<3;j++)
        {
            if(board[i][j]==0) x1++;
            if(board[i][j]==1) o1++;
            if(board[j][i]==0) x2++;
            if(board[j][i]==1) o2++;
            if(board[i][j]==-1) board_full=0;
           
        }
        //if winning row(s)/col(s) found.
        if(x1==3||x2==3) win[0]=1;
        if(o1==3||o2==3) win[1]=1;
            
    }
    //if diagonal cells are all same.
    if(xd1==3||xd2==3) win[0]=1;
    if(od1==3||od2==3) win[1]=1;
    
    if(win[0]==0&&win[1]==0&&board_full==1)//draw
        return 2;
    else if(win[0]==1)//X player wins
        return 0;
    else if(win[1]==1)//O player wins
        return 1;
    else//incomplete game
        return -1;
}

//calls minmax to obtain the best move
void computermove(int board[3][3], int q)
{
    int i, j, score=0, cur_score=-2, row=-1, col=-1;
    
    draw=1;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
        {
            if(board[i][j]==-1)
            {   
                board[i][j]=q;
                score=0;
                score-=minmax(board, (q+1)%2);
                if(score>cur_score)
                {
                    cur_score=score;
                    row=i;
                    col=j;
                }
                board[i][j]=-1;
            }
        }   
    if(draw==1)
    {
        printf("Draw!\n");
        exit(0);
    }
    printf("Computer %s cell number: %d\n", action(q), 3*row+col+1);
    board[row][col]=q;
}

//Minimizes the maximum loss the computer has to suffer from the current game status.
int minmax(int board[3][3], int p)
{
    int i, j, score=0, cur_score=-9, row=-1, col=-1, winner;

    winner=gameStatus(board);
    if(winner!=-1)
    {
        if(winner==0||winner==1)
            draw=-1;
        if(winner==2)
            return 0;
        else if(winner==p)
            return 1;
        else
            return -1;
    }
    
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
        {
            if(board[i][j]==-1)
            {
                board[i][j]=p;
                score=0;
                score-=minmax(board, (p+1)%2);//alternating maximizing/minimizing players
                if(score>cur_score)
                {
                    cur_score=score;
                    row=i;
                    col=j;
                }
                board[i][j]=-1;
            }
        }
    return cur_score;
}
/* */