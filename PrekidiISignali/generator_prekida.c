
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int pid=0;

void prekidna_rutina(int sig){
   kill(pid,SIGKILL);
   /* pošalji SIGKILL procesu 'pid'*/
   exit(0);
}

int main(int argc, char *argv[]){
   int x,y;

   pid=atoi(argv[1]);
   sigset(SIGINT, prekidna_rutina);

   srand(time(NULL));

   while(1){
      /* odspavaj 3-5 sekundi */
      x = (rand() % 3) + 3;
      sleep(x);
      
      /* slučajno odaberi jedan signal (od 4) */
      y = (rand() % 4) + 1;

      /* pošalji odabrani signal procesu 'pid' funkcijom kill*/
      if(y == 1){
         kill(pid,SIGALRM);
      }
      else{
         if(y == 2){
            kill(pid,SIGTERM);
         }
         else{
            if(y==3){
               kill(pid,SIGTRAP);
            }
            else{
               kill(pid,SIGQUIT);
            }
         }
      }
      
   }
   return 0;
}