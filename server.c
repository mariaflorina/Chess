/*
Cod ce are la baza fisierul :  servTcpPreTh.c
https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerPreThread/servTcpPreTh.c
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "board.c"

#define PORT 2000
#define SIZE 1000

struct thread
{
    pthread_t ID;
    int nr_conn;
}; 

struct thread *pool;

int socket_descriptor;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER; 

char move[6];
int cod_val;


void culoare(int client_1, int client_2, long ID)//se trimite culoarea pieselor jucatorilor;
{
    char cul_1='w';
    char cul_2='b';

    /*clientul_1 primeste piesele albe*/
    if (write (client_1, &cul_1, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write() catre clientul 1.\n");
		}
    
    /*clientul_2 primeste piesele negre*/
    if (write (client_2, &cul_2, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write() catre clientul 2.\n");
		}
    

    printf ("Am terminat functia culoare.\n");
    fflush(stdout);

    
}

void ran(int client_1, int client_2, long ID)//se trimite jactorilor cate un cod care indica daca este randl lor sau nu;
{
    char buf[6];
    int nr=0;
    int cod=100;

    /*clientul_1 primeste codul 100 care indica ca este randul sau*/
    if (write (client_1, &cod, sizeof(int)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
    
    cod=101;

    /*clientul_2 primeste codul 101 care indica faptul ca trebuie sa astepte mutarea oponentului*/
    if (write (client_2, &cod, sizeof(int)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}


    printf (" Am terminat functia ran.\n");
    fflush(stdout);

}

int read_move(int client_1, int client_2,long ID)//se citeste actiunea primita de la client;
{
    char buf[6],c='-';


    printf ("Suntem inainte de primul read functia read_move.\n");
    fflush(stdout);

    /*se citeste actiunea clientului_1*/
    if( read(client_1, buf, sizeof(buf)) <= 0 )
    {
		printf("[Thread %ld] : \n",ID);

        /*in caz de eroare, inseamna ca legatura clientului la server s-a pierdut, lucru ce indica faptul ca jocul s-a terminat, iar jucatorul 2 a castigat (primeste codul w)*/
        c='w';
        if(write(client_2,&c,sizeof(char)) <= 0)
        {
            perror("Eroare la write().\n");

        }

        /*returnam -1 pentru a indica terminarea jocului*/
        return -1;
	}

    /*am primit o mutare de la clientul_1 ; jocul continua*/

    if(write(client_2,&c,sizeof(char)) <= 0)
    {
        perror("Eroare la write().\n");

    }

    printf("buf=%s\n",buf);
    fflush(stdout);

    strcpy(move,buf);

    return 0;
}

void validare_mutare(int client_1, int client_2,long ID, char board[][8], int nr)//se valideaza muatarea de la jucator;
{
    char piece, board_2[8][8];
    int poz_xi, poz_yi, poz_xf, poz_yf, cod_mutare=103;

    poz_xi=move[1]-'0';
    poz_yi=move[2]-'0';
    poz_xf=move[3]-'0';
    poz_yf=move[4]-'0';
    piece=board[poz_xi][poz_yi];

    if(possible_road(piece,poz_xi,poz_yi,poz_xf,poz_yf,board))
        {
            if(free_road(piece, poz_xi, poz_yi, poz_xf, poz_yf,board))
                if(free_dest(piece, poz_xi, poz_yi, poz_xf, poz_yf, board))
                    cod_mutare=102;
        }
    
    if(cod_mutare == 102)
    {
        for(int i=0; i<8 ; ++i)
            for(int j=0; j<8 ; ++j)
                board_2[i][j]=board[i][j];

        update_board(board,poz_xi,poz_yi,poz_xf,poz_yf);
        if(nr == 1 )
        {
            if(check(board) == 2)
                {
                    cod_mutare=103;

                    for(int i=0; i<8 ; ++i)
                        for(int j=0; j<8 ; ++j)
                            board[i][j]=board_2[i][j];
                }
        }
        else
        {
             if(check(board) == 1)
                {
                    cod_mutare=103;

                    for(int i=0; i<8 ; ++i)
                        for(int j=0; j<8 ; ++j)
                            board[i][j]=board_2[i][j];
                }
        }
        
    }

    printf("[Thread %ld]Trimitem mesajul inapoi...%d\n",ID, cod_mutare);
	fflush(stdout);
		      
	/* returnam codul de validare clientului_1 */
	if (write (client_1, &cod_mutare, sizeof(int)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	else
    {
		printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
        fflush(stdout);
    }

    /* returnam codul de validare clientului_2 */
	if (write (client_2, &cod_mutare, sizeof(int)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	else
    {
		printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
        fflush(stdout);
    }

    cod_val=cod_mutare;
    
    printf (" Am terminat functia validare_mutare.\n");
    fflush(stdout);
}

void send_move(int client_1, int client_2, long ID, char move[])//este trimisa mutarea valida clientilor;
{
    if (write (client_1, move, 6) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}

	else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }

    if (write (client_2, move, 6) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }
    
    printf (" Am terminat functia send_move.\n");
    fflush(stdout);
}

bool finish_game(int client_1, int client_2, long ID, char board[][8])//se verifica daca jocul a ajuns la final;
{
    char fin;
    if(checkmate(board) == 1)
    {
        fin='w';

        /*se trimite codul de invigator, w, clientului_1*/
        if (write (client_1, &fin, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	    else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }

        fin='l';

        /*se trimite codul de invins, l, clientului_2*/
        if (write (client_2, &fin, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	    else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }
        return 1;
    }

    else if(checkmate(board) == 2)
    {
        fin='l';

        /*se trimite codul de invins, l, clientului_1*/
        if (write (client_1, &fin, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	    else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }

        fin='w';

        /*se trimite codul de invingator, w, clientului_2*/
        if (write (client_2, &fin, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	    else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }
        return 1;
    }

    fin='-';

    /*ambii clienti sunt informati ca jocul nu s-a terminat inca*/
    if (write (client_1, &fin, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	    else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }

    if (write (client_2, &fin, sizeof(char)) <= 0)
		{
		 printf("[Thread %ld] ",ID);
		 perror ("[Thread]Eroare la write().\n");
		}
	    else
        {
		    printf ("[Thread %ld]Mesajul a fost trasmis cu succes.\n",ID);	
            fflush(stdout);
        }

    printf (" Am terminat functia finish_game.\n");
    fflush(stdout);

    return 0;
}

void* treat(void * ID)
{
    int client_1, client_2;
    struct sockaddr_in from1, from2;
    int length1, length2;

    bzero(&from1,sizeof(from1));
    fflush(stdout);

    bzero(&from2,sizeof(from2));
    fflush(stdout);

    while(1)
    {
        length1=sizeof(from1);

        length2=sizeof(from2);

        int tura=1, verificare;

        pthread_mutex_lock(&mlock);

        /*acceptarea a cate 2 clienti*/
        client_1=accept(socket_descriptor, (struct sockaddr *) &from1, &length1);
        if(client_1 < 0)
            perror("[Thread] : Eroare la accept. \n");
        
        
        client_2=accept(socket_descriptor, (struct sockaddr *) &from2, &length2);
        if(client_2 < 0)
            perror("[Thread] : Eroare la accept. \n");
        
        pthread_mutex_unlock(&mlock);

        /*threadul capata 2 noi conexiuni*/
        pool[(long)ID].nr_conn+=2;

        /*este stabilita culoarea pieselor si trimisa clientilor*/
        culoare(client_1, client_2, (long)ID);

        char board[8][8];
        bool finish;

        /*este initiata tabla*/
        begin_board(board);

        do
        {
            if(tura%2 == 1)
            {
                ran(client_1,client_2,(long) ID);
                verificare = read_move(client_1,client_2,(long)ID);
                printf("%s\n",move);
                validare_mutare(client_1, client_2,(long) ID,board, 1);
            }
            else
            {
                ran(client_2, client_1, (long) ID);
                verificare = read_move(client_2,client_1,(long)ID);
                printf("%s\n",move);
                validare_mutare(client_2, client_1,(long) ID, board, 2); 
            }

            /*daca jucatorul, a carui rand era, s-a deconectat, jocul se termina*/
            if(verificare == -1)
                finish=1;
            else
            {
                /*daca mutarea este invalida, va ramane randul clientului curent*/
                if(cod_val == 103)
                {
                    --tura;
                }
                else
                {
                    send_move(client_1, client_2,(long)ID, move);
                    finish=finish_game(client_1,client_2,(long) ID, board);
                }

                tura++;
            }

        } while (finish == 0);

        close(client_1);
        close(client_2);
    }
}


void create_thread(long ID)//este creat un thread cu ID-ul indicat;
{
    pthread_create(&pool[ID].ID,NULL,&treat,(void*)ID);
} 

void connection()//pornim serverul si se servesc clientii care se conecteaza
{
    struct sockaddr_in server;
    int op;

    pool = calloc(sizeof( struct thread),SIZE);

    /*este creat socketul*/
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor == -1)
        perror("[server] Eroare la server.\n");

    op=1;
    setsockopt(socket_descriptor,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
    bzero (&server, sizeof (server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

      /* atasam socketul */
    if (bind (socket_descriptor, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
        {
            perror ("[server]Eroare la bind().\n");
            return ;
        }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (socket_descriptor, 2) == -1)
        {
            perror ("[server]Eroare la listen().\n");
            return ;
        }

    long i;
    for(i=0; i<SIZE ;i++) create_thread(i);

	
    /* servim in mod concurent clientii...folosind thread-uri */
    while(1)
        {
	        printf ("[server] SERVER PORNIT \n");
            pause();				
        }

}

int main()
{
    connection();
    return 0;
}
