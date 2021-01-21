#include <malloc.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

int t; //simulacija vremena (trenutno vrijeme)

struct dretva {
	int id; //1, 2, 3, ...
	int p;  //preostalo vrijeme rada
	int prio; //prioritet
	int rasp; //način raspoređivanja (za opcionalni zadatak)
};


#define MAX_DRETVI	5
struct dretva *P[MAX_DRETVI]; //red pripravnih dretvi, P[0] = aktivna dretva

/* podaci o događajima pojave novih poslova - dretvi */
#define DRETVI	6
int nove[DRETVI][5] =
{
	/* trenutak dolaska, id, p, prio, rasp (prio i rasp se koriste ovisno o rasporedivacu) */
	{ 1,  3, 5, 3, 1 }, /* rasp = 0 => PRIO+FIFO; 1 => PRIO+RR */
	{ 3,  5, 6, 5, 1 },
	{ 7,  2, 3, 5, 0 },
	{ 12, 1, 5, 3, 0 },
	{ 20, 6, 3, 6, 1 },
	{ 20, 7, 4, 7, 1 },
};


void ispis_stanja (int ispisi_zaglavlje)
{
	int i;

	if (ispisi_zaglavlje) {
		printf ( "  t    AKT" );
		for (i = 1; i < MAX_DRETVI; i++)
			printf( "     PR%d", i );
		printf("\n");
	}


	printf("%3d ",t);
	for(i = 0; i < MAX_DRETVI; i++){
		if(P[i] != NULL){
			printf ("  %d/%d/%d ",
				 P[i]->id, P[i]->prio, P[i]->p );
		}
		else{
			printf ( "  -/-/- " );
		}
	}
	printf ( "\n");
}

int main (void) {
  int i;
  for (i=0; i<MAX_DRETVI;++i) {
    P[i]=NULL;
  }

  t=0;

  int brojac=0;
  

  while(1){
    if (t==0){
      ispis_stanja(1);
    }else {
      ispis_stanja (0);
    }
    for (i=0; i<DRETVI; i++) {
      if(nove[i][0] == t && brojac<MAX_DRETVI){
				struct dretva* novaDretva = malloc(sizeof(struct dretva));
				novaDretva->id = nove[i][1];
				novaDretva->p = nove[i][2];
				if(brojac==0){
					P[brojac] = novaDretva;
				}
				else{
					for(int i=0;i<MAX_DRETVI;i++){
						if(P[i]==NULL){
							P[i]= novaDretva;
							break;
						}
					}
				}
				if(t > 9){
					printf(" %d -- nova dretva id=%d, p=%d\n",t,
					nove[i][1],nove[i][2]);
				}
				else{
					printf("  %d -- nova dretva id=%d, p=%d\n",t,
					nove[i][1],nove[i][2]);
				}
				
				brojac++;
				ispis_stanja(0);
				
    	}
    }
    if (P[0] != NULL) {
      P[0]->p = P[0]->p -1;
      if (P[0]->p == 0) {
        printf (" Dretva %d zavrsila\n", P[0]->id);
        for (i=0; i<brojac-1; i++) {
          P[i]=P[i+1];
        }
        P[brojac-1]=NULL;
        brojac--;
      }
    }
    t++;
    sleep(1);
  }
}