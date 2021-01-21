#include <stdio.h>
#include <signal.h>
 
#define N 6    /* broj razina proriteta */
 
static int OZNAKA_CEKANJA[N];
static int PRIORITET[N];
static int TEKUCI_PRIORITET;
static void prekidna_rutina( int sig ); 
 
 
int sig[]={SIGALRM, SIGTERM,SIGTRAP,SIGHUP, SIGINT};
void zabrani_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sighold(sig[i]);
}
void dozvoli_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sigrelse(sig[i]);
}


 
void obrada_signala(int i){
    sigset(SIGALRM, prekidna_rutina);
    sigset(SIGTERM, prekidna_rutina);
    sigset(SIGTRAP, prekidna_rutina);
    sigset(SIGHUP, prekidna_rutina);
    sigset(SIGINT, prekidna_rutina);

    switch(i){

        case(1): printf("-  %s  -  -  -  -\n","P");
                 for(int j =1;j<=5;j++){
                    printf("-  %d  -  -  -  -\n",j);
                    sleep(1);
                 }
                 printf("-  %s  -  -  -  -\n","K");
                 break;
        case(2): printf("-  -  %s  -  -  -\n","P");
                 for(int j =1;j<=5;j++){
                    printf("-  -  %d  -  -  -\n",j);
                    sleep(1);
                 }
                 printf("-  -  %s  -  -  -\n","K");
                 break;
        case(3): printf("-  -  -  %s  -  -\n","P");
                 for(int j =1;j<=5;j++){
                    printf("-  -  -  %d  -  -\n",j);
                    sleep(1);
                 }
                 printf("-  -  -  %s  -  -\n","K");
                break;
        case(4): printf("-  -  -  -  %s  -\n","P");
                 for(int j =1;j<=5;j++){
                    printf("-  -  -  -  %d  -\n",j);
                    sleep(1);
                 
                 }
                 printf("-  -  -  -  %s  -\n","K");
                 break;
        case(5): printf("-  -  -  -  -  %s\n","P");
                 for(int j =1;j<=5;j++){
                    printf("-  -  -  -  -  %d\n",j);
                    sleep(1);
                 }
                 printf("-  -  -  -  -  %s\n","K");
                 break;



    }
}

void prekidna_rutina(int sig){
   int n=-1;
   int j = 1;
   int i = 1;
   zabrani_prekidanje();
   switch(sig){
      case SIGALRM: 
         n=1; 
         printf("-  X  -  -  -  -\n");
         break;
      case SIGTERM: 
         n=2; 
         printf("-  -  X  -  -  -\n");
         break;
      case SIGTRAP:
         n=3;
         printf("-  -  -  X  -  -\n");
         break;
      case SIGHUP:
         n=4;
         printf("-  -  -  -  X  -\n");
         break;
      case SIGINT:
         n=5;
         printf("-  -  -  -  -  X\n");
         break;
   }
   
  OZNAKA_CEKANJA[n] +=1;
  do{
    j = 0;
    for(i=TEKUCI_PRIORITET+1; i<N; i++){
      if(OZNAKA_CEKANJA[i] != 0)
        j = i;
    }

    if(j > 0){
      OZNAKA_CEKANJA[j]= OZNAKA_CEKANJA[j] - 1;
      PRIORITET[j] = TEKUCI_PRIORITET;
      TEKUCI_PRIORITET = j;
      dozvoli_prekidanje;
      obrada_signala(j);
      zabrani_prekidanje;
      TEKUCI_PRIORITET = PRIORITET[j];
    }
  }while (j > 0);
  
  dozvoli_prekidanje();


}


 
int main ( void )
{
   sigset (SIGALRM, prekidna_rutina);
   sigset (SIGTERM, prekidna_rutina);
   sigset (SIGTRAP, prekidna_rutina);
   sigset (SIGQUIT, prekidna_rutina);
   sigset (SIGINT, prekidna_rutina);
 
   printf("Proces obrade prekida, PID=%ld\n", getpid());
   printf("GP S1 S2 S3 S4 S5\n");
   printf("-----------------\n");

   
   for(int i=0;i<=9;i++){
      sleep(1);
      printf("%d  -  -  -  -  -\n",i);

   }

   for(int i=0;i<=9;i++){
      sleep(1);
      printf("%d  -  -  -  -  -\n",i);

   }
   for(int i=0;i<=9;i++){
      sleep(1);
      printf("%d  -  -  -  -  -\n",i);

   }
   for(int i=0;i<=9;i++){
      sleep(1);
      printf("%d  -  -  -  -  -\n",i);

   }

   for(int i=0;i<=9;i++){
      sleep(1);
      printf("%d  -  -  -  -  -\n",i);

   }
   for(int i=0;i<=9;i++){
      sleep(1);
      printf("%d  -  -  -  -  -\n",i);

   }
 
   printf ("Zavrsio osnovni program\n");
 
   return 0;
}