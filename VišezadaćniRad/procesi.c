#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
int Id; /* identifikacijski broj segmenta */
int *A;
 
void povecajZaJedan()
{
   *A = *A + 1;
}

void procitajVrijednostA()
{
	int i = *A;
 	printf("A=%d\n",i);
}
void unistiSegment(int sig)
{
   /* oslobađanje zajedničke memorije */
   (void) shmdt((int *) A);
   (void) shmctl(Id, IPC_RMID, NULL);
   exit(0);
}
int main(int argc, char *argv[])
{
   /* zauzimanje zajedničke memorije */
   Id = shmget(IPC_PRIVATE, sizeof(int), 0600);
 
   if (Id == -1)
      exit(1);  /* greška - nema zajedničke memorije */
 
   A = (int *) shmat(Id, NULL, 0);
   *A = 0;
   sigset(SIGINT, unistiSegment);//u slučaju prekida briši memoriju
   int N,M;

   
   N = atoi(argv[1]);
   M = atoi(argv[2]);
 

   int i;

   for(i=0;i <N;i++){
   		switch(fork()){
   			case 0:
   				for(int j=0;j<M;j++){
   					povecajZaJedan();
   				}
   				exit(0);
   			case -1:
   				printf("Nije se uspio stvoriti proces");
   				exit(0);
 
   		}
   }

   while(i--) wait(NULL);
 
   procitajVrijednostA();

   unistiSegment(0);


   
 
   return 0;
}