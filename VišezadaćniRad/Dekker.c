#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>


int PRAVO=1;
int ZASTAVICA[2];
long int A;
int M;

void povecajZaJedan(){
   for(int i=0;i<M;i++){
      A+=1;
   }
}

void Citac(){
   printf("A=%ld\n",A);
}
 
void udi_u_kriticni_odsjecak(int i,int j)
{
   ZASTAVICA[i] = 1;
   while (ZASTAVICA[j] != 0) {
      if (PRAVO==j) {
         ZASTAVICA[i] = 0;
         while (PRAVO==j);
         ZASTAVICA[i] = 1;
      }
   }
}
 
void izadi_iz_kriticnog_odsjecka(int i,int j)
{
   PRAVO = j;
   ZASTAVICA[i] = 0;
}

void proc(int *i){ 
   int broj = *i;
   int pm = 1;
   if(broj ==1){
      pm = 0;
   }

   int brojac = 1;    
   do{
      udi_u_kriticni_odsjecak(broj,pm);
      povecajZaJedan();
      izadi_iz_kriticnog_odsjecka(broj,pm);
      brojac--;
   }while(brojac > 0);
}

void zaustavi(){
   exit(0);
}


int main(int argc, char *argv[])

{
   int i=0, j=1;


   pthread_t thread1, thread2;
   M = atoi(argv[1]);

   sigset(SIGINT, zaustavi);

   A = 0;

   if (pthread_create(&thread1, NULL, (void *)proc, (void *)&i) != 0) {
        printf("Greska pri stvaranju dretve!\n");
        exit(1);
   }
   if (pthread_create(&thread2, NULL, (void *)proc, (void *)&j) != 0) {
        printf("Greska pri stvaranju dretve!\n");
        exit(1);
   }

   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);

   Citac();


   return 0;
}