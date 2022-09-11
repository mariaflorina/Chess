/*
Cod ce are la baza fisierul :  cliTcpNr.c
https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerPreThread/cliTcpNr.c

*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h> 
#include "board.c"

extern int errno;

int port=2000;
char *ad_serv="192.168.0.109";

int sd;			            // descriptorul de socket
struct sockaddr_in server;	// structura folosita pentru conectare 


char board[8][8];

int play()//desfasurarea jocului
{
    int cod_m,nr,cod_valid;
    int ok=1;
    char result,c;

    printf("\n");
    fflush(stdout);
    print_board(board);//afisam tabla de joc la fiecare tura;
    fflush(stdout);

    /*verificare rand jucator*/
    if(read(sd,&cod_m,sizeof(cod_m)) <= 0)
    {
        perror ("[client]Conexiunea cu celalalt jucator pierduta! Joc abandonat!\n");
        return errno;
    }

    /*se trimit informatiile unei mutari posibile*/
    if(cod_m == 100)
    {
         /* citire mutare jucator */
        printf ("\n[client]Introduceti mutarea: ");

        fflush (stdout);

        char buf[6];
        read (0, buf, sizeof(buf));
        strtok(buf,"\n");

        /* trimiterem mutarea la server */
        if (write (sd,buf,sizeof(buf)) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }


    }

    else
    {
        /*este randul adversarului si asteptam*/
        printf ("\n[client]Se asteapta mutarea facuta de celalalt client!\n");
        fflush (stdout);

        /*citim codul care ne indica daca adversarul a iesit din joc*/
        if(read(sd,&c,sizeof(c))<= 0)
            {
                perror ("[client]Eroare la read() de la server.\n");
                return errno;
            }
            
        if(c=='w')
            {
                printf("Win\n");
                fflush(stdout);
                return 0;
            }

    }

    /* citirea daca mutarea e valida sau nu (apel blocant pina cind serverul raspunde) */
    if (read (sd, &cod_valid,sizeof(int)) <= 0)
    {
        perror ("[client]Conexiunea cu celalalt jucator pierduta! Joc abandonat!\n");
        return errno;
    }
    
    /* afisam codul de validitate */
    printf ("[client]Codul primit este: %d\n", cod_valid);
    fflush (stdout);

    /*Punem jucatorul sa reintroduca mutarea in cazul in care aceasta e invalida*/
    if(cod_valid == 103)
    {
        printf ("[client]Mutare invalida! Trebuie reintrodusa mutarea.\n");
        fflush (stdout);
        return 1;
    }

    else
    {
        char mutare[6];

        /*Se trimite mutarea valida ambilor clienti*/
        if (read (sd, mutare,sizeof(mutare)) <= 0)
            {
                perror ("[client]Conexiunea cu celalalt jucator pierduta! Joc abandonat!\n");
                return errno;
            }
        
        strtok(mutare,"\n");
           
        /* afisam mutarea valida primita */
        printf ("[client]Mutarea valida este: %s\n", mutare);
        fflush (stdout);

        /*verificam daca e checkmate*/
        if (read (sd, &result,sizeof(result)) <= 0)
            {
                perror ("[client]Conexiunea cu celalalt jucator pierduta! Joc abandonat!\n");
                return errno;
            }
           
        /* afisam daca e checkmate */
        printf ("[client]Rezultat: %c\n", result);
        fflush (stdout);

        update_board(board,(mutare[1]-'0'),(mutare[2]-'0'),(mutare[3]-'0'),(mutare[4]-'0'));


        /*decidem daca terminam jocul sau continua*/
        if(result!='-')
            return 0;
        return 1;
    }
    return 1;
}


int main ()
{
    char colour;

    /* cream socketul */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(ad_serv);
    /* portul de conectare */
    server.sin_port = htons (port);
  
    /* ne conectam la server */
    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
        {
            perror ("[client]Eroare la connect().\n");
            return errno;
        }

    /*aflam culoarea pieselor clientului*/
    if(read(sd,&colour,sizeof(colour)) <= 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }

    if(colour=='w')
        printf("\n[client]Culoarea pieselor : Alb (Litere mari)\n");
    else
        printf("\n[client]Culoarea pieselor : Negru (Litere mici)\n");
    
    fflush (stdout);

    begin_board(board);//se initiaza tabla de sah;
    fflush (stdout);

    while(play());

    /* inchidem conexiunea, am terminat */
    close (sd);
    
    return 0;
}
