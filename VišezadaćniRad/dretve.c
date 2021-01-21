#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

long int A;
int N,M;

void *Pisac()
{
   for(int i=0;i<M;i++){
      A+=1;
   }
}

void Citac()
{
  printf("A=%ld\n",A);
}


int main(int argc, char *argv[])
{
   pthread_t *thr_id=NULL;
   N = atoi(argv[1]);
   M = atoi(argv[2]);

   thr_id = (pthread_t*) malloc (N*sizeof(pthread_t));

   A = 0;

   int j;
   for(j=0;j<N;j++){
      if(pthread_create(&thr_id[j], NULL, Pisac, NULL)!=0){
         printf("Greska pri stvaranju dretve!\n");
         exit(1);
      }
   }
   for(j=0;j<N;j++){
      pthread_join(thr_id[j],NULL);
   }

   Citac();


   return 0;
}