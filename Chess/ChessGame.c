/**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int checkcorrectness(int r11, int c11, int r22, int c22, int boardcopy[8][8]);
int checklegality(int r11, int c11, int r22, int c22, int boardcopy[8][8]);
int pawnpromotion(int boardcopy[8][8]);
void computermove(int boardcopy[8][8]);
void humanmove(int boardcopy[8][8]);
int checkkingcheck(int board[8][8], int sign);
int checkmate(int boardcopy[8][8], char player);
int value(int c);
char* piece_name(int v);
int is_legalmove(int row1, int col1, int row2, int col2, char color, int boardcopy[8][8]);
void print_board(int boardcopy[8][8]);
int minmax(int is_maxplayer, int alpha, int beta, int depth, int boardcopy[8][8]);
int evaluation(int board[8][8]);

int whitekingmoved, blackkingmoved, rook07moved, rook00moved, rook77moved, rook70moved;
const int nr[8]={2, 2, -2, -2, 1, 1, -1, -1}, nc[8]={1, -1, 1, -1, 2, -2, 2, -2};
const int qr[8]={-1, -1, 1, 1, -1, 1, 0, 0}, qc[8]={-1, 1, -1, 1, 0, 0, -1, 1};

const int knighthash[8][8]= {-50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0, 5, 5, 0, -20, -40, -30, 5, 10, 15, 15, 10, 5, -30, -30, 0, 15, 20, 20, 15, 0, -30, -30, 5, 15, 20, 20, 15, 5, -30, -30, 0, 10, 15, 15, 10, 0, -30, -40, -20, 0, 0, 0, 0, -20, -40, -50, -40, -30, -30, -30, -30, -40, -50 };
const int bishophash[8][8]= {-20, -10, -10, -10, -10, -10, -10, -20, -10, 5, 0, 0, 0, 0, 5, -10, -10, 10, 10, 10, 10, 10, 10, -10, -10, 0, 10, 10, 10, 10, 0, -10, -10, 5, 5, 10, 10, 5, 5, -10, -10, 0, 5, 10, 10, 5, 0, -10, -10, 0, 0, 0, 0, 0, 0, -10, -20, -10, -10, -10, -10, -10, -10, -20};
const int rookhash[8][8]= {0, 0, 0, 5, 5, 0, 0, 0, -5, 0, 0, 0, 0, 0, 0, -5, -5, 0, 0, 0, 0, 0, 0, -5, -5, 0, 0, 0, 0, 0, 0, -5, -5, 0, 0, 0, 0, 0, 0, -5, -5, 0, 0, 0 ,0 ,0, 0, -5, 5, 10, 10, 10, 10, 10, 10, 5, 0, 0 ,0, 0, 0, 0, 0, 0};
const int queenhash[8][8]= {-20, -10, -10, -5, -5, -10, -10, -20, -10, 0, 5, 0, 0, 0, 0, -10, -10, 5, 5, 5, 5, 5, 0, -10, 0, 0, 5, 5, 5, 5, 0, -5, -5, 0, 5, 5, 5, 5, 0, -5, -10, 0, 5, 5, 5, 5, 0, -10, -10, 0, 0, 0, 0 ,0, 0, -10, -20, -10, -10, -5, -5, -10, -10, -20};
const int pawnhash[8][8]={0, 0, 0, 0, 0, 0, 0, 0, 5, 10, 10, -20, -20, 10, 10, 5, 5, -5, -10, 0, 0, -10, -5 , 5, 0, 0, 0, 20, 20, 0, 0, 0, 5, 5, 10, 25, 25, 10, 5, 5, 10, 10, 20, 30, 30, 20, 10, 10, 50, 50, 50, 50, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0};
const int kinghash[8][8]= {20, 30, 10, 0, 0, 10, 30, 20, 20, 20, 0, 0, 0, 0, 20, 20, -10, -20, -20, -20, -20, -20, -20, -10, -20, -30, -30, -40, -40, -30, -30, -20, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30};
const int kingendhash[8][8]={-50, -30, -30, -30, -30, -30, -30, -50, -30, -30, 0, 0, 0, 0, -30, -30, -30, -10, 20, 30, 30, 20, -10, -30, -30, -10, 30, 40, 40, 30, -10, -30, -30, -10, 30, 40, 40, 30, -10, -30, -30, -10, 20, 30, 30, 20, -10, -30, -30, -20, -10, 0, 0, -10, -20, -30, -50, -40, -30, -20, -20, -30, -40, -50};
const int king=20000, queen=900, bishop=330, knight=320, rook=500, pawn=100;
char whoplays, *name;
int playercolor, target=0;

struct lastmove
{
    int en_pass, row2, col2;
}opplastmove[3];

void print_board(int boardcopy[8][8])
{
    int a, b;
    printf("\n");
    printf("## ## ***************************************************************** ## ##\n");
    printf("  *   |   A   |   B   |   C   |   D   |   E   |   F   |   G   |   H   |   *  \n");
    printf("## ## ***************************************************************** ## ##\n");
    
    for(a=0; a<8; a++)
    {	
        printf("*****                                                                   *****\n");
        printf("| %d |\t", a+1);
        for(b=0; b<8; b++)
        {
            if(boardcopy[a][b]>0)
                printf("w%s\t", piece_name(boardcopy[a][b]));
            else if(boardcopy[a][b]<0)
                printf("b%s\t", piece_name(boardcopy[a][b]));
            else
                printf("0\t");
        }
        printf("| %d |\n", a+1);
        printf("*****                                                                   *****\n");
    }
    printf("## ## ***************************************************************** ## ##\n");
    printf("  *   |   A   |   B   |   C   |   D   |   E   |   F   |   G   |   H   |   *  \n");
    printf("## ## ***************************************************************** ## ##\n");
}

int main()
{
    char q;
    int exit_game=0;
    
    int board[8][8]={rook,knight,bishop,queen,king,bishop,knight,rook, pawn,pawn,pawn,pawn,pawn,pawn,pawn,pawn, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-pawn,-pawn,-pawn,-pawn,-pawn,-pawn,-pawn,-pawn,-rook,-knight,-bishop,-queen,-king,-bishop,-knight,-rook};
    //int board[8][8]={rook,0,0,0,0,rook, king,0,0,pawn,pawn,0,0,pawn,pawn,0,pawn,0,knight,0,0,0,0,pawn,-pawn,0,0,0,0,queen,0,0,0,0,0,-pawn,pawn,bishop,0,0,0,0,-bishop,0,0,-pawn,0,0,bishop,-pawn,-pawn,0,-queen,0,0,0,0,-king,0,-rook,0,0,-rook,0};
    print_board(board);
    printf("Choose your color (w/b): ");
    scanf(" %c", &q);
    if(q=='w'||q=='W') 
    {   playercolor=1;
        whoplays='h';
    }
    else 
    {   playercolor=-1;
        whoplays='c';
    }
    whitekingmoved=0;
    blackkingmoved=0;
    rook00moved=0;
    rook07moved=0;
    rook70moved=0;
    rook77moved=0;

    opplastmove[0].en_pass=-1;
    opplastmove[0].row2=-1;
    opplastmove[0].col2=-1;
    opplastmove[2].en_pass=-1;
    opplastmove[2].row2=-1;
    opplastmove[2].col2=-1;
    
    do
    {
        if(whoplays=='c')
        {   
            printf("\nThinking...\n");
            computermove(board);
            print_board(board);
        }
        else if(whoplays=='h')
        {   
            printf("Do you wanna quit?(y/n): ");
            do{
            scanf(" %c", &q);
            }while(q!='n'&&q!='N'&&q!='y'&&q!='Y');
            if(q=='y'||q=='Y')
                exit(0);
            else if(q=='n'||q=='N')
            {
                humanmove(board);
                print_board(board);
            }
        }
        
        if(target!=0)
        {
            name=piece_name(target);
            if(whoplays=='c')
                printf("Computer lost a %s\n", name);
            else
                printf("You lost a %s\n", name);
        }
        
        if(checkmate(board, whoplays)==1)
        {
            if(whoplays=='c')
                printf("Checkmate! You win!");
            else
                printf("Checkmate! Computer wins!");
            exit_game=1;
        }
        
        else
        {
            if(whoplays=='h'&&checkkingcheck(board,playercolor)==1)
                printf("Check!\n");
            else if(whoplays=='c'&&checkkingcheck(board,-1*playercolor)==1)
                printf("Check, Chessie!\n");
        } 
    }while(exit_game==0);
}

void humanmove(int board[8][8])
{
    int r1, c1, r2, c2, movingpiece, pawnpromoted=-1;
    char pos[10];
    
    do{
        printf("Enter the original position: ");
        scanf("%s", pos);
        int len = strlen(pos);
        r1 = pos[len-1]-49;
        c1 = pos[len-2];
        c1 = (c1<97)?c1-65:c1-97;
        
    }while(r1<0||r1>9||c1<0||c1>9);
    
    do{
        printf("Enter the destination position: ");
        scanf("%s", pos);
        int len = strlen(pos);
        r2 = pos[len-1]-49;
        c2 = pos[len-2];
        c2 = (c2<97)?c2-65:c2-97;
        
    }while(r2<0||r2>9||c2<0||c2>9);
    
    if(((playercolor==1)&&(board[r1][c1]>0))||((playercolor==-1)&&(board[r1][c1]<0)))
    {	
        //if valid, make the move.
        if(checkcorrectness(r1, c1, r2, c2, board)==1)
        {   
            movingpiece=board[r1][c1];
            board[r1][c1]=0;
            
            //saving captured piece for display later
            target=board[r2][c2];
            
            board[r2][c2]=movingpiece;
            
            //updating castling data
            if(movingpiece==king)
                whitekingmoved=1;
            else if(movingpiece==-king)
                blackkingmoved=1;
            else if((movingpiece==rook)&&(r1==0)&&(c1==7))
                rook07moved=1;
            else if((movingpiece==rook)&&(r1==0)&&(c1==0))
                rook00moved=1;
            else if((movingpiece==-rook)&&(r1==7)&&(c1==7))
                rook77moved=1;
            else if((movingpiece==-rook)&&(r1==7)&&(c1==0))
                rook70moved=1;
            
            //En passant moves
            if((movingpiece==playercolor*pawn)&&(abs(r2-r1)==1&&abs(c2-c1)==1)&&board[r2][c2]==0)
                board[r2-playercolor][c2]=0;
                
            if(movingpiece==playercolor*pawn&&abs(r2-r1)==2)
                opplastmove[1+playercolor].en_pass=1;
            else
                opplastmove[1+playercolor].en_pass=-1;
            
            
            if((movingpiece==king||movingpiece==-king)&&(c2-c1==2))//moving the rook if castling occurred 
            {   board[r2][5]=board[r2][7];
                board[r2][7]=0;
                if(r2==0) rook07moved=1;
                else rook77moved=1;
            }
            else if((movingpiece==king||movingpiece==-king)&&(c1-c2==2))
            {   board[r2][3]=board[r2][0];
                board[r2][0]=0;
                if(r2==0) rook00moved=1;
                else rook70moved=1;
            }
        
            opplastmove[1+playercolor].row2=r2;
            opplastmove[1+playercolor].col2=c2;
            
            pawnpromoted=pawnpromotion(board);
            
        }
        else
        {   printf("Invalid move!\n");
            humanmove(board);
        }
    }
    else
    {	printf("Not your piece!\n");
        humanmove(board);
    }
    whoplays='c';
}

int checkcorrectness(int r11, int c11, int r22, int c22, int boardcopy[8][8])
{
    int movingpiece=boardcopy[r11][c11];
    int sign = (movingpiece>0)?1:-1;
    int correctness=0;
    
    if(sign*movingpiece==rook)
    {   if(((r11==r22)&&(c11!=c22))||((r11!=r22)&&(c11==c22)))
            correctness=1;
        else return 0;
    }
    if(sign*movingpiece==knight)
    for(int i=0;i<8;i++)
    {   if((r22==r11+nr[i])&&(c22==c11+nc[i]))
            correctness=1;
    }
    if(sign*movingpiece==bishop)
    {   if((abs(r11-r22)==abs(c11-c22))&&(r11!=r22))
            correctness=1;
    }
    if(sign*movingpiece==queen)
    {   if((abs(r11-r22)==abs(c11-c22))&&(r11!=r22))
            correctness=1;
        else if((r11==r22)&&(c11!=c22))
            correctness=1;
        else if((r11!=r22)&&(c11==c22))
            correctness=1;
        
    }
    if(sign*movingpiece==king)//Normal king moves
    {   if((r11==r22)&&(abs(c11-c22)==1))
            correctness=1;
        else if((abs(r11-r22)==1)&&(c11==c22))
            correctness=1;
        else if((abs(r11-r22)==1)&&(abs(c11-c22)==1))
            correctness=1;
    }
    
    if((movingpiece==king)&&(r11==r22)&&(r11==0)&&(abs(c11-c22)==2))//White king Castling move
    {
        if(c11-c22>0)//queen side castling
        {
            if(rook00moved==0&&whitekingmoved!=1)
            {
                if(boardcopy[0][0]==rook&&boardcopy[0][1]==0&&boardcopy[0][2]==0&&boardcopy[0][3]==0)
                    correctness=1;
            }
        }
        else if(c11-c22<0)//king side castling
        {
            if(rook07moved==0&&whitekingmoved!=1)
            {
                if(boardcopy[0][7]==rook&&boardcopy[0][5]==0&&boardcopy[0][6]==0)
                    correctness=1;
            }
        }
    }
    
    if((movingpiece==-king)&&(r11==r22)&&(r11==7)&&(abs(c11-c22)==2))//Black king Castling move
    {
        if(c11-c22>0)//queen side castling
        {
            if(rook70moved==0&&blackkingmoved!=1)
            {
                if((boardcopy[7][0]==-rook)&&(boardcopy[7][1]==0)&&(boardcopy[7][2]==0)&&(boardcopy[7][3]==0))
                    correctness=1;
            }
        }
        else if(c11-c22<0)//king side castling
        {
            if(rook77moved==0&&blackkingmoved!=1)
            {
                if((boardcopy[7][7]==-rook)&&(boardcopy[7][5]==0)&&(boardcopy[7][6]==0))
                   correctness=1;
            }
        }
    }
    
    if(movingpiece*sign==pawn)
    {   if((r22==r11+1*sign)&&(c22==c11))
            correctness=1;
        else if((r22==r11+2*sign)&&(c22==c11)&&(r11==(7+sign)%7))
            correctness=1;
        else if((r22==r11+1*sign)&&((c22==c11+1)||(c22==c11-1)))
        {   if(sign*boardcopy[r22][c22]<0)
                correctness=1;
            //validation of En passant move
            else if(boardcopy[r22][c22]==0&&boardcopy[r22-sign][c22]==-pawn*sign)
            {
                if(opplastmove[1-sign].en_pass==1&&opplastmove[1-sign].row2==(r22-sign)&&opplastmove[1-sign].col2==c22)
                    correctness=1;
            }
        }
    }
    
    if(correctness==1)
        return checklegality(r11, c11, r22, c22, boardcopy);
    return 0;
}


int checklegality(int r11, int c11, int r22, int c22, int boardcopy[8][8])
{	
    int u, i, j, movingpiece, temp, kingcheck=0, sign;
    
    movingpiece=boardcopy[r11][c11];
    sign = (movingpiece>0)?1:-1;
    
    temp=boardcopy[r22][c22];
    boardcopy[r22][c22]=movingpiece;
    boardcopy[r11][c11]=0;

    kingcheck=checkkingcheck(boardcopy, sign);
    
    boardcopy[r11][c11]=movingpiece;
    boardcopy[r22][c22]=temp;
    
    if(kingcheck==1)
        return 0;
        
    if(sign*boardcopy[r22][c22]>0)
        return 0;
    
    //If castling move, the square being crossed over must not be in check as well
    if((movingpiece==king||movingpiece==-king)&&abs(c11-c22)==2)
    {                                                           	
        boardcopy[r11][c11]=0;
        if(c22-c11>0)
            temp=c22-1;
        else
            temp=c22+1;
        boardcopy[r11][temp]=movingpiece;
        
        kingcheck=checkkingcheck(boardcopy, sign);
        boardcopy[r11][c11]=movingpiece;
        boardcopy[r22][temp]=0;
        if(kingcheck==1)
            return 0;
    }
    
    //non-capturing pawn moves
    else if(sign*movingpiece==pawn&&c22==c11)
    {
        for(u=r11+sign;u!=r22+sign;u+=sign)
            if(boardcopy[u][c22]!=0)
                return 0;
    }
    
    else if(sign*movingpiece==rook)
    {   for(u=4;u<8;u++)
        {   if((r22-r11)==qr[u]*abs(r22-r11)&&(c22-c11)==qc[u]*abs(c22-c11))
            {   for(i=r11+qr[u],j=c11+qc[u];i!=r22||j!=c22;i+=qr[u],j+=qc[u])
                {   if(boardcopy[i][j]!=0)
                        return 0;
                }
            }
        }
    }
    else if(sign*movingpiece==bishop)
    {   for(u=0;u<4;u++)
        {   if((r22-r11)==qr[u]*abs(r22-r11)&&(c22-c11)==qc[u]*abs(c22-c11))
            {   for(i=r11+qr[u],j=c11+qc[u];i!=r22||j!=c22;i+=qr[u],j+=qc[u])
                {   if(boardcopy[i][j]!=0)
                        return 0;
                }
            }
        }
    }
    else if(sign*movingpiece==queen)
    {   
        if(r22==r11||c22==c11)
        for(u=4;u<8;u++)
        {   if((r22-r11)==qr[u]*abs(r22-r11)&&(c22-c11)==qc[u]*abs(c22-c11))
            {   for(i=r11+qr[u],j=c11+qc[u];i!=r22||j!=c22;i+=qr[u],j+=qc[u])
                {   
                    if(boardcopy[i][j]!=0)
                        return 0;
                }
            }
        }
        else
        for(u=0;u<4;u++)
        {   if((r22-r11)==qr[u]*abs(r22-r11)&&(c22-c11)==qc[u]*abs(c22-c11))
            {   for(i=r11+qr[u],j=c11+qc[u];i!=r22||j!=c22;i+=qr[u],j+=qc[u])
                {   
                    if(boardcopy[i][j]!=0)
                        return 0;
                }
            }
        }
    }
    return 1;
}

int pawnpromotion(int board[8][8])
{	
    int x, v, row=7, pawnpromoted=-1;
    if(playercolor==-1) row=0;
    for(v=0; v<8; v++)
    {	if(board[row][v]==playercolor*pawn)
        {   
            pawnpromoted=1;
            printf("Which piece do you want your pawn to promote to? \n 1. Rook \n 2. Bishop \n 3. Knight \n 4. Queen \n");
            scanf("%d", &x);
            switch (x)
            {   case 1: board[row][v]=playercolor*rook;
                        break;
                case 2: board[row][v]=playercolor*bishop;
                        break;
                case 3: board[row][v]=playercolor*knight;
                        break;
                case 4: board[row][v]=playercolor*queen;
                        break;
            }
        }
    }
    return pawnpromoted;
}

int checkkingcheck(int board[8][8], int ssign)
{
    int i, j, u, v, kingrow=-1, kingcolumn=-1, sign;
    for(u=0; u<8; u++)
    {	for(v=0; v<8; v++)
        {
            if(board[u][v]==ssign*king)
            {	kingrow=u;
                kingcolumn=v;
                //printf("kingrow=%d\n", kingrow);
            }
        }
    }
    sign=-ssign;
    
    if(kingrow==-1)
    {
        //printf("board[7][0]=%d board[7][1]=%d\n", board[7][0], board[7][1]);
        //printf("Couldn't find the king!\n");
        return 1;
    }

    for(int i=0;i<4;i++)
    {
        for(u=kingrow+qr[i],v=kingcolumn+qc[i];u<8&&u>=0&&v<8&&v>=0;u+=qr[i], v+=qc[i])
            if(board[u][v]!=0)
            {
                if(board[u][v]==sign*queen||board[u][v]==sign*bishop) return 1;
                else break;
            }
    }
    for(int i=4;i<8;i++)
    {
        for(u=kingrow+qr[i],v=kingcolumn+qc[i];u<8&&u>=0&&v<8&&v>=0;u+=qr[i], v+=qc[i])
            if(board[u][v]!=0)
            {
                if(board[u][v]==sign*queen||board[u][v]==sign*rook) return 1;
                else break;
            }
    }
    if((kingrow-1>=0)&&(kingcolumn+1<8))
        if(board[kingrow-1][kingcolumn+1]==sign*pawn)
            return 1;
    if((kingrow-1>=0)&&(kingcolumn-1>=0))
        if(board[kingrow-1][kingcolumn-1]==sign*pawn)
            return 1;

    for(i=0;i<8;i++)
    {
        if(kingrow+nr[i]<8&&kingrow+nr[i]>=0&&kingcolumn+nc[i]<8&&kingcolumn+nc[i]>=0)
            if(board[kingrow+nr[i]][kingcolumn+nc[i]]==sign*knight)
                return 1;
    }
    
    for(i=0;i<8;i++)
    {
        if(board[kingrow+qr[i]][kingcolumn+qc[i]]==sign*king)
            return 1;
    }
    
    return 0;
}

int checkmate(int board[8][8], char player)
{	
    int colour, i, j, k, l;
    if(player=='h')
        colour=playercolor;
    else if(player=='c')
    {
        if(playercolor==1)
            colour=-1;
        else
            colour=1;
    }
    for(i=0;i<8;i++)
    for(j=0;j<8;j++)
    {
        if((colour==1&&board[i][j]>0)||(colour==-1&&board[i][j]<0))
        for(k=0;k<8;k++)
        for(l=0;l<8;l++)
        {
            if(checkcorrectness(i, j, k, l, board)==1)
                return -1;//No checkmate
        }
    }
    return 1;
}

void computermove(int board[8][8])
{
    int iii, jjj, kkk, lll, srow=-1, scol=-1, frow=-1, fcol=-1, tp, mp, en_pawn=0, newscore, bestscore=-99999;
    int color, enpass, r, c;
    if(playercolor==1)
        color=-1;
    else
        color=1;
    
    enpass=opplastmove[1+color].en_pass;
    r=opplastmove[1+color].row2;
    c=opplastmove[1+color].col2;

    for(iii=0;iii<8;iii++)
    for(jjj=0;jjj<8;jjj++)
    if(((color==-1)&&(board[iii][jjj]<0))||((color==1)&&(board[iii][jjj]>0)))
    {	
        for(kkk=0;kkk<8;kkk++)
        for(lll=0;lll<8;lll++)
        {   
            if(checkcorrectness(iii, jjj, kkk, lll, board)==1)
            {
                //make the move
                mp=board[iii][jjj];
                tp=board[kkk][lll];
                board[iii][jjj]=0;
                board[kkk][lll]=mp;

                //Castling moves
                if(mp==king&&(lll-jjj==2))
                {
                    board[0][7]=0;
                    board[0][5]=rook;
                    whitekingmoved=1;
                    rook07moved=1;
                }
                if(mp==king&&(jjj-lll==2))
                {
                    board[0][0]=0;
                    board[0][3]=rook;
                    whitekingmoved=1;
                    rook00moved=1;
                }
                if(mp==-king&&(lll-jjj==2))
                {
                    board[7][7]=0;
                    board[7][5]=-rook;
                    blackkingmoved=1;
                    rook77moved=1;
                }
                if(mp==-king&&(jjj-lll==2))
                {
                    board[7][0]=0;
                    board[7][3]=-rook;
                    blackkingmoved=1;
                    rook70moved=1;
                }
                
                //En passant moves
                if((mp==color*pawn)&&(abs(kkk-iii)==1&&abs(lll-jjj)==1)&&board[kkk][lll]==0)
                {   
                    en_pawn=board[kkk-color][lll];
                    board[kkk-color][lll]=0;
                }
                
                if(mp==pawn*color&&abs(kkk-iii)==2)
                    opplastmove[1+color].en_pass=1;
                else
                    opplastmove[1+color].en_pass=-1;
                
                //pawn promotion
                if((mp==-pawn)&&(kkk==0))
                    board[kkk][lll]=-queen;
                
                else if((mp==pawn)&&(kkk==7))
                    board[kkk][lll]=queen;
                
                opplastmove[1+color].row2=kkk;
                opplastmove[1+color].col2=lll;
                
                newscore=minmax(-1, -99999, 99999, 3, board);//calculate how good this move is.
                
                if(newscore>bestscore)//if better, store this one as the best so far.
                {   bestscore=newscore;
                    srow=iii;
                    scol=jjj;
                    frow=kkk;
                    fcol=lll;
                }
                //undo the move 
                board[iii][jjj]=mp;
                board[kkk][lll]=tp;
                
                opplastmove[1+color].en_pass=enpass;
                opplastmove[1+color].row2=r;
                opplastmove[1+color].col2=c;
                
                //Undo Castling
                if(mp==king&&(lll-jjj==2))
                {
                    board[0][7]=rook;
                    board[0][5]=0;
                    whitekingmoved=0;
                    rook07moved=0;
                }
                if(mp==king&&(jjj-lll==2))
                {
                    board[0][0]=rook;
                    board[0][3]=0;
                    whitekingmoved=0;
                    rook00moved=0;
                }
                if(mp==-king&&(lll-jjj==2))
                {
                    board[7][7]=-rook;
                    board[7][5]=0;
                    blackkingmoved=0;
                    rook77moved=0;
                }
                if(mp==-king&&(jjj-lll==2))
                {
                    board[7][0]=-rook;
                    board[7][3]=0;
                    blackkingmoved=0;
                    rook70moved=0;
                }           
                //undo en-passant capture
                if(en_pawn!=0)
                {
                    board[kkk-color][lll]=en_pawn;
                    en_pawn=0;
                }
            }
        }
    }
    //if no move was found, simply return.
    if(srow==-1)
    {   
        printf("No move found! You win!\n");
        whoplays='h';
        return;
    }
    
    //else, make the best move found.
    mp=board[srow][scol];
    printf("%s will move!\n", piece_name(mp));
    
    target=board[frow][fcol];
    board[srow][scol]=0;
    board[frow][fcol]=mp;
    
    //Castling move
    if(mp==king&&srow==0&&scol==4)
    {
        if(frow==0&&fcol==6)
        {
            board[0][5]=rook;
            board[0][7]=0;
        }
        else if(frow==0&&fcol==2)
        {
            board[0][3]=rook;
            board[0][0]=0;
        }
    }
    if(mp==-king&&srow==7&&scol==4)
    {
        if(frow==7&&fcol==6)
        {
            board[7][5]=-rook;
            board[7][7]=0;
        }
        else if(frow==7&&fcol==2)
        {
            board[7][3]=-rook;
            board[7][0]=0;
        }        
    }
    
    //capturing the En passant pawn
    if(mp==color*pawn&&abs(frow-srow)==1&&abs(fcol-scol)==1&&board[frow][fcol]==0)
    {
        target=board[frow-color][fcol];
        board[frow-color][fcol]=0;
    }
    if(mp==color*pawn&&abs(frow-srow)==2)
        opplastmove[1+color].en_pass=1;
    else
        opplastmove[1+color].en_pass=-1;

    //updating castling data
    if(mp==king)
        whitekingmoved=1;
    else if(mp==-king)
        blackkingmoved=1;
    else if((mp==rook)&&(srow==0)&&(scol==7))
        rook07moved=1;
    else if((mp==rook)&&(srow==0)&&(scol==0))
        rook00moved=1;
    else if((mp==-rook)&&(srow==7)&&(scol==7))
        rook77moved=1;
    else if((mp==-rook)&&(srow==7)&&(scol==0))
        rook70moved=1;
    
    //pawn promotion
    if((mp==-pawn)&&(frow==0))
        board[frow][fcol]=-queen;
        
    else if((mp==pawn)&&(frow==7))
        board[frow][fcol]=queen;
    
    //saving the latest computer move.
    opplastmove[1+color].row2=frow;
    opplastmove[1+color].col2=fcol;
    
    whoplays='h';
}

char* piece_name(int v)
{
    int sign=(v<0)?-1:1;
    if(v==sign*knight)
        return "Knight";
    else if(v==sign*bishop)
        return "Bishop";
    else if(v==sign*rook)
        return "Rook";
    else if(v==sign*queen)
        return "Queen";
    else if(v==sign*king)
        return "King";
    else if(v==sign*pawn)
        return "Pawn";
}

int evaluation(int board[8][8])
{
    int score=0, i, j, army_strength=0, opp_army=0;
    
    for(i=0;i<8;i++)
    {    for(j=0;j<8;j++)
        {   if((playercolor==-1&&board[i][j]>0)||(playercolor==1&&board[i][j]<0))
            {
                if(board[i][j]==-playercolor*bishop)
                    score+=bishophash[i][j];
                else if(board[i][j]==-playercolor*knight)
                    score+=knighthash[i][j];
                else if(board[i][j]==-playercolor*rook)
                    score+=rookhash[i][j];
                else if(board[i][j]==-playercolor*pawn)
                    score+=pawnhash[i][j];
                else if(board[i][j]==-playercolor*queen)
                    score+=queenhash[i][j];
                else if(board[i][j]==-playercolor*king)
                    score+=kinghash[i][j];
                army_strength+=abs(board[i][j]);
            }
            else
                opp_army+=abs(board[i][j]);
        }
    }

    score+=army_strength;
    score-=opp_army;
    
    return score;
}


int minmax(int is_maxplayer, int alpha, int beta, int depth, int boardcopy[8][8])
{
    int s_row, s_col, f_row, f_col, movingpiece, temp2, en_pawn=0;
    int tempscore, cur_score, color, t1, t2, t3;
    
    
    if(checkmate(boardcopy, 'c')==1)
        return evaluation(boardcopy)-90000;
  
    if(checkmate(boardcopy, 'h')==1)
        return evaluation(boardcopy)+90000;
    
    if(depth==0)
        return evaluation(boardcopy);
    
    if(is_maxplayer==-1)//if player's turn
    {   color=playercolor;
        cur_score=99999;
    }
    else if(is_maxplayer==1)
    {	if(playercolor==1)
            color=-1;
        else
            color=1;
        cur_score=-99999;
    }
    
    t1=opplastmove[1+color].row2;
    t2=opplastmove[1+color].col2;
    t3=opplastmove[1+color].en_pass;

    for(s_row=0;s_row<8;s_row++)
    for(s_col=0;s_col<8;s_col++)
    if((color==1&&boardcopy[s_row][s_col]>0)||(color==-1&&boardcopy[s_row][s_col]<0))
    {	for(f_row=0;f_row<8;f_row++)
        for(f_col=0;f_col<8;f_col++)
        if(checkcorrectness(s_row, s_col, f_row, f_col, boardcopy)==1)
        {       
            //make the move
            movingpiece=boardcopy[s_row][s_col];
            temp2=boardcopy[f_row][f_col];
            boardcopy[s_row][s_col]=0;
            boardcopy[f_row][f_col]=movingpiece;

            //Castling moves
            if(movingpiece==king&&(f_col-s_col==2))
            {
                boardcopy[0][7]=0;
                boardcopy[0][5]=rook;
                whitekingmoved=1;
                rook07moved=1;
            }
            if(movingpiece==king&&(s_col-f_col==2))
            {
                boardcopy[0][0]=0;
                boardcopy[0][3]=rook;
                whitekingmoved=1;
                rook00moved=1;
            }
            if(movingpiece==-king&&(f_col-s_col==2))
            {
                boardcopy[7][7]=0;
                boardcopy[7][5]=-rook;
                blackkingmoved=1;
                rook77moved=1;
            }
            if(movingpiece==-king&&(s_col-f_col==2))
            {
                boardcopy[7][0]=0;
                boardcopy[7][3]=-rook;
                blackkingmoved=1;
                rook70moved=1;
            }
            
            //En passant moves
            if(movingpiece==pawn*color&&abs(f_row-s_row)==1&&abs(f_col-s_col)==1&&boardcopy[f_row][f_col]==0)
            {   
                en_pawn=boardcopy[f_row-color][f_col];
                boardcopy[f_row-color][f_col]=0;
            }
            if(movingpiece==pawn*color&&abs(f_row-s_row)==2)
                opplastmove[1+color].en_pass=1;
            else
                opplastmove[1+color].en_pass=-1;
            
            //pawnpromotion
            if((movingpiece==-pawn)&&(f_row==0))
                boardcopy[f_row][f_col]=-queen;
            
            else if((movingpiece==pawn)&&(f_row==7))
                boardcopy[f_row][f_col]=queen;
            
            opplastmove[1+color].row2=f_row;
            opplastmove[1+color].col2=f_col;

            tempscore=minmax(is_maxplayer*-1, alpha, beta, depth-1, boardcopy);

            boardcopy[s_row][s_col]=movingpiece;
            boardcopy[f_row][f_col]=temp2;
            
            opplastmove[1+color].row2=t1;
            opplastmove[1+color].col2=t2;
            opplastmove[1+color].en_pass=t3;
            
            //Undo Castling
            if(movingpiece==king&&(f_col-s_col==2))
            {
                boardcopy[0][7]=rook;
                boardcopy[0][5]=0;
                whitekingmoved=0;
                rook07moved=0;
            }
            if(movingpiece==king&&(s_col-f_col==2))
            {
                boardcopy[0][0]=rook;
                boardcopy[0][3]=0;
                whitekingmoved=0;
                rook00moved=0;
            }
            if(movingpiece==-king&&(f_col-s_col==2))
            {
                boardcopy[7][7]=-rook;
                boardcopy[7][5]=0;
                blackkingmoved=0;
                rook77moved=0;
            }
            if(movingpiece==-king&&(s_col-f_col==2))
            {
                boardcopy[7][0]=-rook;
                boardcopy[7][3]=0;
                blackkingmoved=0;
                rook70moved=0;
            }
            
            //undo en-passant capture
            if(en_pawn!=0)
            {
                boardcopy[f_row-color][f_col]=en_pawn;
                en_pawn=0;
            }
               
            if(is_maxplayer==1)
            {
                if(tempscore>=beta)
                    return beta;
                if(tempscore>alpha)
                    alpha=tempscore;
            }
            else
            {
                if(tempscore<=alpha)
                    return alpha;
                if(tempscore<beta)
                    beta=tempscore;
            }
        }
    }
    if(is_maxplayer==1)
        return alpha;
    else
        return beta;
}
/* */