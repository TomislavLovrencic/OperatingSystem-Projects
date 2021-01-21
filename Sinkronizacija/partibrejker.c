#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m;


pthread_cond_t red[2];

int brojStudenata;
int partibrejkerJeUSobi;

int studentUdi;
int partiBrejker;
int N;
int brojStudenataUSustavu;
int j;

void *student(void  *k)
{
	int broj = *((int *) k);
 	int x = rand()%401 + 100;
 	usleep(1000*x);
  
  for(int i=0;i<3;i++) {
      
   	  pthread_mutex_lock(&m);
      
   	  while(partibrejkerJeUSobi == 1){
   	  	pthread_cond_wait(&red[studentUdi], &m);
   	  }
      pthread_cond_broadcast(&red[studentUdi]);

   	  brojStudenata = brojStudenata + 1;
   	  printf("Student %d je ušao u sobu\n",broj);
      
   	  if(brojStudenata > 2){
   	  	pthread_cond_signal(&red[partiBrejker]);
   	  }
   	  pthread_mutex_unlock(&m);

      x = rand()%1001 + 1000;
      usleep(1000*x);

      pthread_mutex_lock(&m);
     
      brojStudenata = brojStudenata - 1;
      printf("Student %d je izašao iz sobe\n",broj);
      if(brojStudenata == 0){
      	pthread_cond_signal(&red[partiBrejker]);
      }
      //printf("----------------------->%d\n",i);
      pthread_mutex_unlock(&m);
      if(i == 2){
        //printf("dsadsa")
        brojStudenataUSustavu--;
        //printf("%d\n",brojStudenataUSustavu);
      }
      //printf("%d--------------------%d\n",i,broj);
      
      x = rand()%1001 + 1000; 
      usleep(1000*x);
   
   }
}



void  *partibrejker()
{
   while(brojStudenataUSustavu > 0) {
   		int x = rand()%901 + 100;
   		usleep(x*1000);
     
      pthread_mutex_lock(&m);
   		while(brojStudenata <= 2 && brojStudenataUSustavu > 0){
   			pthread_cond_wait(&red[partiBrejker], &m);
   		}
      if(brojStudenata > 2){
        partibrejkerJeUSobi = 1;
        printf("Partibrejker je ušao u sobu\n");
      }
   		
      while(brojStudenata > 0){
        pthread_cond_wait(&red[partiBrejker],&m);
      }
      if(brojStudenata == 0 && brojStudenataUSustavu > 0){
          partibrejkerJeUSobi = 0;
          printf("Partibrejker je izašao iz sobe\n");
          pthread_cond_broadcast(&red[studentUdi]);
      }
      
      pthread_mutex_unlock(&m);
      
   }
}

int main ()
{

  pthread_mutex_init (&m, NULL);
 
 
  pthread_cond_init (&red[0], NULL);
  pthread_cond_init (&red[1], NULL);
  
  srand ((unsigned int) time(NULL));

  
  printf("Unesite broj studenata > ");
  scanf("%d",&N);


  brojStudenataUSustavu = N;
  brojStudenata = 0;
  partibrejkerJeUSobi = 0;
  studentUdi = 0;
  partiBrejker = 1;
  j = 1;

  pthread_t thread_id[N + 1];
   int brojevi[N];
    for (int i = 0; i < N; i++) {
        brojevi[i] = i+1;
        pthread_create (&thread_id[i], NULL, student, &brojevi[i]);
    }

    pthread_create(&thread_id[N], NULL, partibrejker, NULL);
        
    for (int j = 0; j < N+1; j++){
      pthread_join (thread_id[j], NULL); 
    }
    pthread_join (thread_id[N], NULL);
   
  pthread_mutex_destroy (&m);
  pthread_cond_destroy (&red[0]);
  pthread_cond_destroy (&red[1]);

  return 0;
}

