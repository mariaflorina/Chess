#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> 

bool possible_road(char piece, int place_xi, int place_yi, int place_xf, int place_yf,char board[][8])//the piece is allowed to move in that direction
{
    char p2=piece;

    switch(toupper(p2))
    {
        case 'P':
            if(piece=='P')
            {
                if( place_xf == place_xi - 1 )
                {
                    if(( place_yf == place_yi && board[place_xf][place_yf]=='-' ) || ( board[place_xf][place_yf]>='a'&& place_yf == place_yi + 1) || ( board[place_xf][place_yf]>='a' && place_yf == place_yi - 1))
                        return 1;
                    else return 0;
                }
                else if(place_xf == place_xi - 2)
                {
                    if( place_yf == place_yi && board[place_xf][place_yf]=='-' && place_xi == 6)
                        return 1;
                    else return 0;
                }
                else return 0;
            }
            else
            {
                if( place_xf == place_xi + 1 )
                {
                    if(( place_yf == place_yi && board[place_xf][place_yf]=='-' ) || ( board[place_xf][place_yf]<='Z' && board[place_xf][place_yf]>='A' && place_yf == place_yi + 1) || ( board[place_xf][place_yf]>='A' && board[place_xf][place_yf]<='Z' && place_yf == place_yi - 1) )
                        return 1;
                    else return 0;
                }
                 else if(place_xf == place_xi + 2)
                {
                    if( place_yf == place_yi && board[place_xf][place_yf]=='-' && place_xi == 1)
                        return 1;
                    else return 0;
                }
                else return 0;
            }
        case 'R':
            if(place_xf == place_xi || place_yf==place_yi)
                return 1;
            else return 0;
        case 'K':
            if( (place_xf == place_xi+2) || (place_xf == place_xi-2) )
            {
                if( (place_yf == place_yi+1) || (place_yf==place_yi-1) )
                    return 1;
                else 
                    return 0;
            }
            else if( (place_xf == place_xi+1) || (place_xf == place_xi-1) )
            {
                if( (place_yf == place_yi+2) || (place_yf==place_yi-2) )
                    return 1;
                else 
                    return 0;
            }
            else return 0;
        case 'B':
            if(abs(place_xf - place_xi) == abs(place_yf - place_yi))
                return 1;
            else return 0;
        case 'Q':
            if(place_xf == place_xi || place_yf==place_yi)
                return 1;
            else if(abs(place_xf - place_xi) == abs(place_yf - place_yi))
                return 1;
            else return 0;
        case 'G':
            if(abs(place_xf - place_xi) <= 1 && abs(place_yf - place_yi) <= 1)
                return 1;
            else return 0;
    }
    return 0;
}


bool free_road(char piece, int place_xi, int place_yi, int place_xf, int place_yf,char board[][8])//check if the path until the final destination is free;
{
    int i,j,xi,yi,xf,yf;
    char p2=piece;
    switch(toupper(piece))
    {
        case 'R':
            if(place_xf == place_xi)
                {
                    if(place_yf>place_yi)
                    {
                        yi=place_yi;
                        yf=place_yf;
                    }
                    else
                    {
                        yf=place_yi;
                        yi=place_yf;
                    }
                    
                    for( j = yi+1 ; j< yf; ++j)
                        if(board[place_xi][j]!='-')
                            return 0;
                    return 1;
                }
            else
                {
                    if(place_xf>place_xi)
                    {
                        xi=place_xi;
                        xf=place_xf;
                    }
                    else
                    {
                        xf=place_xi;
                        xi=place_xf;
                    }
   
                    for( i = place_xi+1 ; i< place_xf; ++i)
                        if(board[i][place_yi]!='-')
                            return 0;
                    return 1;
                }
        case 'B':
            if(place_xf - place_xi > 0)
            {
                xi=place_xi;
                xf=place_xf;
                if(place_yf - place_yi > 0)
                {
                    yi=place_yi;
                    yf=place_yf;

                }
                else
                {
                    yi=place_yf;
                    yf=place_yi;
                }
                for(i=xi+1,j=yi+1;i<xf;++i,++j)
                    if(board[i][j]!='-')
                        return 0;
                return 1;
            }
            else
            {
                xi=place_xf;
                xf=place_xi;
                if(place_yf - place_yi > 0)
                {
                    yi=place_yi;
                    yf=place_yf;

                }
                else
                {
                    yi=place_yf;
                    yf=place_yi;
                }
                for(i=xi+1,j=yi+1;i<xf;++i,++j)
                    if(board[i][j]!='-')
                        return 0;
                return 1;
            }
        case 'Q':
            if(place_xf == place_xi || place_yf==place_yi)
            {
                if(place_xf == place_xi)
                {
                    if(place_yf>place_yi)
                    {
                        yi=place_yi;
                        yf=place_yf;
                    }
                    else
                    {
                        yf=place_yi;
                        yi=place_yf;
                    }
                    
                    for( j = yi+1 ; j< yf; ++j)
                        if(board[place_xi][j]!='-')
                            return 0;
                    return 1;
                }
                else
                {
                    if(place_xf>place_xi)
                    {
                        xi=place_xi;
                        xf=place_xf;
                    }
                    else
                    {
                        xf=place_xi;
                        xi=place_xf;
                    }
   
                    for( i = place_xi+1 ; i< place_xf; ++i)
                        if(board[i][place_yi]!='-')
                            return 0;
                    return 1;
                }
            }
            else
            {
               if(place_xf - place_xi > 0)
                {
                    xi=place_xi;
                    xf=place_xf;
                    if(place_yf - place_yi > 0)
                    {
                        yi=place_yi;
                        yf=place_yf;
                    }
                    else
                    {
                        yi=place_yf;
                        yf=place_yi;
                    }
                    for(i=xi+1,j=yi+1;i<xf;++i,++j)
                        if(board[i][j]!='-')
                            return 0;
                    return 1;
                }
                else
                {
                    xi=place_xf;
                    xf=place_xi;
                    if(place_yf - place_yi > 0)
                    {
                        yi=place_yi;
                        yf=place_yf;
                    }
                    else
                    {
                        yi=place_yf;
                        yf=place_yi;
                    }   
                    for(i=xi+1,j=yi+1;i<xf;++i,++j)
                        if(board[i][j]!='-')
                            return 0;
                    return 1;
                } 
            }
        default:
            return 1;
    }
}


bool free_dest(char piece, int place_xi, int place_yi, int place_xf, int place_yf,char board[][8])//check if there is not a piece of the same colour at the destination;
{
    if(board[place_xf][place_yf]=='-')
        return 1;
    else if(piece>='a')
    {
        if(board[place_xf][place_yf]>='a')
            return 0;
        return 1;
    }
    else
    {
        if(board[place_xf][place_yf]<='Z')
            return 0;
        return 1;
    }
}

bool check(char board[][8])//check if the kings are in danger;
{
    int xkw,ykw,xkb,ykb,i,j;
    for(i=0; i<8 ; ++i)
        for(j=0; j<8 ; ++j)
            if(board[i][j]=='G')
            {
                xkw=i;
                ykw=j;
            }
            else if(board[i][j]=='g')
            {
                xkb=i;
                ykb=j;
            }
   for(i=0; i<8 ; ++i)
        for(j=0; j<8 ; ++j) 
        {
            if(board[i][j] == 'R' || board[i][j] == 'B' || board[i][j] == 'Q')
                if(possible_road(board[i][j],i,j,xkb,ykb,board))
                {
                    if(free_road(board[i][j],i,j,xkb,ykb,board))
                    {
                    return 1;
                    }
                }   
            if(board[i][j] == 'P' || board[i][j] == 'K' || board[i][j] == 'G')
                if(possible_road(board[i][j],i,j,xkb,ykb,board))
                {
                   return 1;
                }
            if(board[i][j] == 'r' || board[i][j] == 'b' || board[i][j] == 'q')
                if(possible_road(board[i][j],i,j,xkw,ykw,board))
                {
                    if(free_road(board[i][j],i,j,xkw,ykw,board))
                    {
                        return 2;
                    }
                }
            if(board[i][j] == 'p' || board[i][j] == 'k' || board[i][j] == 'g')
                if(possible_road(board[i][j],i,j,xkw,ykw,board))
                {
                   return 2;
                }
        } 
    return 0;   
}

bool move_king(char board[][8])//check if the king can move in another place;
{
    int xkw,ykw,xkb,ykb,i,j;
    for(i=0; i<8 ; ++i)
        for(j=0; j<8 ; ++j)
            if(board[i][j]=='G')
            {
                xkw=i;
                ykw=j;
            }
            else if(board[i][j]=='g')
            {
                xkb=i;
                ykb=j;
            }
    if(check(board)==2)
    {
        if(xkw + 1 < 8)
        {
            if(ykw + 1 < 8)
            {
                board[xkw+1][ykw+1]='G';
                board[xkw][ykw]='-';
                if(check(board)!=2)
                    return 1;
                board[xkw][ykw]='G';
                board[xkw+1][ykw+1]='-';
            }

            board[xkw+1][ykw]='G';
            board[xkw][ykw]='-';
            if(check(board)!=2)
                return 1;
            board[xkw][ykw]='G';
            board[xkw+1][ykw]='-';

            if(ykw - 1 >= 0)
            {
                board[xkw+1][ykw-1]='G';
                board[xkw][ykw]='-';
                if(check(board)!=2)
                    return 1;
                board[xkw][ykw]='G';
                board[xkw+1][ykw-1]='-';
            }
        }

        if(ykw + 1 < 8)
            {
                board[xkw][ykw+1]='G';
                board[xkw][ykw]='-';
                if(check(board)!=2)
                    return 1;
                board[xkw][ykw]='G';
                board[xkw][ykw+1]='-';
            }

            board[xkw][ykw]='G';
            board[xkw][ykw]='-';
            if(check(board)!=2)
                return 1;
            board[xkw][ykw]='G';
            board[xkw][ykw]='-';

            if(ykw - 1 >= 0)
            {
                board[xkw][ykw-1]='G';
                board[xkw][ykw]='-';
                if(check(board)!=2)
                    return 1;
                board[xkw][ykw]='G';
                board[xkw][ykw-1]='-';
            }
        if(xkw - 1 >= 0)
        {
            if(ykw + 1 < 8)
            {
                board[xkw-1][ykw+1]='G';
                board[xkw][ykw]='-';
                if(check(board)!=2)
                    return 1;
                board[xkw][ykw]='G';
                board[xkw-1][ykw+1]='-';
            }

            board[xkw-1][ykw]='G';
            board[xkw][ykw]='-';
            if(check(board)!=2)
                return 1;
            board[xkw][ykw]='G';
            board[xkw-1][ykw]='-';

            if(ykw - 1 >= 0)
            {
                board[xkw-1][ykw-1]='G';
                board[xkw][ykw]='-';
                if(check(board)!=2)
                    return 1;
                board[xkw][ykw]='G';
                board[xkw-1][ykw-1]='-';
            }
        }
        return 0;  

    }

    else if(check(board) == 1)
    {
        if(xkb + 1 < 8)
        {
            if(ykb + 1 < 8)
            {
                board[xkb+1][ykb+1]='g';
                board[xkb][ykb]='-';
                if(check(board)!=1)
                    return 1;
                board[xkb][ykb]='g';
                board[xkb+1][ykb+1]='-';
            }

            board[xkb+1][ykb]='g';
            board[xkb][ykb]='-';
            if(check(board)!=1)
                return 1;
            board[xkb][ykb]='g';
            board[xkb+1][ykb]='-';

            if(ykb - 1 >= 0)
            {
                board[xkb+1][ykb-1]='g';
                board[xkb][ykb]='-';
                if(check(board)!=1)
                    return 1;
                board[xkb][ykb]='g';
                board[xkb+1][ykb-1]='-';
            }
        }

        if(ykb + 1 < 8)
            {
                board[xkb][ykb+1]='g';
                board[xkb][ykb]='-';
                if(check(board)!=1)
                    return 1;
                board[xkb][ykb]='g';
                board[xkb][ykb+1]='-';
            }

            board[xkb][ykb]='g';
            board[xkb][ykb]='-';
            if(check(board)!=1)
                return 1;
            board[xkb][ykb]='g';
            board[xkb][ykb]='-';

            if(ykb - 1 >= 0)
            {
                board[xkb][ykb-1]='g';
                board[xkb][ykb]='-';
                if(check(board)!=1)
                    return 1;
                board[xkb][ykb]='g';
                board[xkb][ykb-1]='-';
            }
        if(xkb - 1 >= 0)
        {
            if(ykb + 1 < 8)
            {
                board[xkb-1][ykb+1]='g';
                board[xkb][ykb]='-';
                if(check(board)!=1)
                    return 1;
                board[xkb][ykb]='g';
                board[xkb-1][ykb+1]='-';
            }

            board[xkb-1][ykb]='g';
            board[xkb][ykb]='-';
            if(check(board)!=1)
                return 1;
            board[xkb][ykb]='g';
            board[xkb-1][ykb]='-';

            if(ykb - 1 >= 0)
            {
                board[xkb-1][ykb-1]='g';
                board[xkb][ykb]='-';
                if(check(board)!=1)
                    return 1;
                board[xkb][ykb]='g';
                board[xkb-1][ykb-1]='-';
            }
        }  

        return 0;
    }
    else
        return 1;
}

bool block(char board[][8])//check if the piece that attacks the king can be blocked
{
    int xkw,ykw,xkb,ykb,i,j,xa,ya,ok=-1,ii,jj;
    bool pos = 0;
    for(i=0; i<8 ; ++i)
        for(j=0; j<8 ; ++j)
            if(board[i][j]=='G')
            {
                xkw=i;
                ykw=j;
            }
            else if(board[i][j]=='g')
            {
                xkb=i;
                ykb=j;
            }
     for(i=0 ; i<8 ; ++i)
            for(j=0 ; j<8 ; ++j)
            {
                if(board[i][j] == 'R' || board[i][j] == 'B' || board[i][j] == 'Q')
                    if(possible_road(board[i][j],i,j,xkb,ykb,board))
                        {
                            if(free_road(board[i][j],i,j,xkb,ykb,board))
                            {
                                xa=i;
                                ya=j;
                                i=8;
                                j=8;
                                ok=0;
                            }
                        }
                if(board[i][j] == 'r' || board[i][j] == 'b' || board[i][j] == 'q')
                    if(possible_road(board[i][j],i,j,xkw,ykw,board))
                        {
                            if(free_road(board[i][j],i,j,xkw,ykw,board))
                            {
                                xa=i;
                                ya=j;
                                i=8;
                                j=8;
                                ok=1;
                            }
                        }
            }          
    for(i=0; i<8 && !pos; ++i)
        for(j=0 ;j<8 && !pos; ++j)
            if(ok==0)
            {
                if(board[i][j]>='a' && board[i][j]!='g')
                {
                    for(ii=0 ; ii<8 && !pos ; ++ii)
                            for(jj=0 ; jj<8 && !pos; ++jj)
                                if(board[ii][jj]=='-')
                                {
                                    if(possible_road(board[i][j],i,j,ii,jj,board))
                                        if(free_road(board[i][j],i,j,ii,jj,board))
                                        {
                                            board[ii][jj]=board[i][j];
                                            board[i][j]='-';

                                            if(free_road(board[xa][ya],xa,ya,xkb,ykb,board)==0)
                                                pos=1;

                                            board[i][j]=board[ii][jj];
                                            board[ii][jj]='-';
                                        }
                                }
                    
                }
            }
            else
            {
                if(board[i][j]<='Z' && board[i][j]!='G')
                {
                    for(ii=0 ; ii<8 && !pos ; ++ii)
                            for(jj=0 ; jj<8 && !pos; ++jj)
                                if(board[ii][jj]=='-')
                                {
                                    if(possible_road(board[i][j],i,j,ii,jj,board))
                                        if(free_road(board[i][j],i,j,ii,jj,board))
                                        {
                                            board[ii][jj]=board[i][j];
                                            board[i][j]='-';

                                            if(free_road(board[xa][ya],xa,ya,xkw,ykw,board) == 0)
                                                pos=1;

                                            board[i][j]=board[ii][jj];
                                            board[ii][jj]='-';
                                        }
                                }
                    
                }
            }
    return pos;
}

bool eliminate(char board[][8])//check if the piece that attacks the king can be eliminate
{
     int xkw,ykw,xkb,ykb,i,j,xa,ya,ok=-1,ii,jj;
    bool pos = 0;
    for(i=0; i<8 ; ++i)
        for(j=0; j<8 ; ++j)
            if(board[i][j]=='G')
            {
                xkw=i;
                ykw=j;
            }
            else if(board[i][j]=='g')
            {
                xkb=i;
                ykb=j;
            }
    for(i=0 ; i<8 ; ++i)
            for(j=0 ; j<8 ; ++j)
            {
                if(board[i][j] == 'R' || board[i][j] == 'B' || board[i][j] == 'Q')
                    if(possible_road(board[i][j],i,j,xkb,ykb,board))
                        {
                            if(free_road(board[i][j],i,j,xkb,ykb,board))
                            {
                                xa=i;
                                ya=j;
                                i=8;
                                j=8;
                                ok=0;
                            }
                        }
                if(board[i][j] == 'P' || board[i][j] == 'K' )
                        if(possible_road(board[i][j],i,j,xkb,ykb,board))
                        {
                            xa=i;
                            ya=j;
                            i=8;
                            j=8;
                            ok=0;
                        }
                if(board[i][j] == 'r' || board[i][j] == 'b' || board[i][j] == 'q')
                    if(possible_road(board[i][j],i,j,xkw,ykw,board))
                        {
                            if(free_road(board[i][j],i,j,xkw,ykw,board))
                            {
                                xa=i;
                                ya=j;
                                i=8;
                                j=8;
                                ok=1;
                            }
                        }
                if(board[i][j] == 'p' || board[i][j] == 'k' )
                    if(possible_road(board[i][j],i,j,xkw,ykw,board))
                    {
                        xa=i;
                        ya=j;
                        i=8;
                        j=8;
                        ok=1;
                   
                    }
            }          
    for(i=0; i<8 && !pos; ++i)
        for(j=0 ;j<8 && !pos; ++j)
            if(ok==0)
            {
                if(board[i][j]>='a')
                {
                    if(possible_road(board[i][j],i,j,xa,ya,board))
                        if(free_road(board[i][j],i,j,xa,ya,board))
                            pos=1;
                    
                }
            }
            else
            {
                if(board[i][j]<='Z')
                {
                   if(possible_road(board[i][j],i,j,xa,ya,board))
                        if(free_road(board[i][j],i,j,xa,ya,board))
                            pos=1;
                    
                }
            }
    return pos;

}

bool checkmate(char board[][8])//verify if there are any possible moves left
{
    if(check(board)==1)
    {
        if(move_king(board))
                return 0;
        if(block(board))
                return 0;
        if(eliminate(board))
                return 0;
        return 1;        
    }
    else if(check(board)==2)
    {
        if(move_king(board))
            return 0;
        if(block(board))
            return 0;
        if(eliminate(board))
            return 0;
        return 2;  
    }
    return 0; 
}