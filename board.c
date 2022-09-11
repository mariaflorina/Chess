#include <stdio.h>
#include <stdlib.h>
#include "rules.c"

void begin_board(char board[][8])//create the start board;
{
    int i,j;
    for(i=0 ; i<8 ; ++i )
        for(j = 0 ; j<8 ; ++j)
            board[i][j]='-';

    board[0][0]=board[0][7]='r';
    board[0][1]=board[0][6]='k';
    board[0][2]=board[0][5]='b';
    board[0][3]='q';
    board[0][4]='g';

    board[7][0]=board[7][7]='R';
    board[7][1]=board[7][6]='K';
    board[7][2]=board[7][5]='B';
    board[7][3]='Q';
    board[7][4]='G';

    for(j=0 ; j<8 ; ++j)
    {
        board[1][j]='p';
        board[6][j]='P';
    }
}

void update_board(char board[][8],int poz_xi, int poz_yi, int poz_xf, int poz_yf)//update the board with the current move;
{
    board[poz_xf][poz_yf]=board[poz_xi][poz_yi];
    board[poz_xi][poz_yi]='-';
}

void print_board(char board[][8])//print the board;
{
    int i,j;
    for(i=0; i<8 ; ++i)
        {
            for(j=0; j<8 ; ++j)
                printf("%c ",board[i][j]);
            printf("\n");
        }
    printf("\n");
}

/*
int main()
{
    char board[8][8];
    begin_board(board);
    print_board(board);

    if(possible_road(board[6][4],6,4,5,4,board))
    {
        update_board(board,6,4,5,4);
    }

    if(possible_road(board[1][5],1,5,2,5,board))
    {
        update_board(board,1,5,2,5);
    }

    if(possible_road(board[5][4],5,4,4,4,board))
    {
        update_board(board,5,4,4,4);
    }

    if(possible_road(board[2][5],2,5,3,5,board))
    {
        update_board(board,2,5,3,5);
    }

    if(possible_road(board[4][4],4,4,3,5,board))
    {
        update_board(board,4,4,3,5);
    }

    if(possible_road(board[1][6],1,6,2,6,board))
    {
        update_board(board,1,6,2,6);
    }

    if(possible_road(board[6][3],6,3,5,3,board))
    {
        update_board(board,6,3,5,3);
    }

    if(possible_road(board[2][6],2,6,3,6,board))
    {
        update_board(board,2,6,3,6);
    }

    if(possible_road(board[2][6],2,6,3,6,board))
    {
        update_board(board,2,6,3,6);
    }

    if(possible_road(board[7][3],7,3,3,7,board))
    {
        update_board(board,7,3,3,7);
    }

    printf("%d\n",check(board));
    printf("%d\n\n",checkmate(board));
    print_board(board);
    return 0;
}
*/

