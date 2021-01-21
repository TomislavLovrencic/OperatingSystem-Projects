#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

sem_t *KO;

int *s1 = 0;

sem_t *p1;
sem_t *p2;
sem_t *p3;
sem_t *stol_prazan;

void trgovac() {
	do {
		*s1 = rand()%(3-1+1) +1;
		//(s1, s2) = nasumice_odaberi_dva_različita_sastojka
		sem_wait(KO);
		if(*s1 == 1) {
			printf("Trgovac stavlja duhan i šibice\n");
			sem_post(p1);
		}
		if(*s1 == 2){
			printf("Trgovac stavlja duhan i papir\n");
			sem_post(p2);
		} 
		if(*s1 == 3){
			printf("Trgovac stavlja papir i šibice\n");
			sem_post(p3);
		} 
		sem_post(KO);
		sem_wait(stol_prazan);
		sleep(1);
	} while(1);
}

void pusac(int p) {

	do {
		if (p == 1) {
			sem_wait(p1);
			sem_wait(KO);
			printf("Pusac %d: Uzima sastojke i ...\n\n",p);
			sem_post(KO);
			sem_post(stol_prazan);
		}
		if(p == 2){
			sem_wait(p2);
			sem_wait(KO);
			printf("Pusac %d: Uzima sastojke i ...\n\n",p);
			sem_post(KO);
			sem_post(stol_prazan);
		}
		if(p == 3){
			sem_wait(p3);
			sem_wait(KO);
			printf("Pusac %d: Uzima sastojke i ...\n\n",p);
			sem_post(KO);
			sem_post(stol_prazan);
		}
		else {
			sem_post(KO);
		}
	} while(1);
}


int main ()
{
	int ID,i;
	pid_t pid;
	srand(time(NULL));

	ID = shmget (IPC_PRIVATE, sizeof(sem_t) + sizeof(int), 0600);
	KO = shmat(ID, NULL, 0);
	p1 = (sem_t *) (KO +1);
	p2 = (sem_t *) (KO +2);
	p3 = (sem_t *) (KO +3);
	stol_prazan = (sem_t *) (KO +4);
	s1 = (int *) (KO + 5);

	shmctl(ID, IPC_RMID, NULL); //moze odmah ovdje, nakon shmat

	sem_init(KO, 1, 1);
	sem_init(p1,1,0);
	sem_init(p2,1,0);
	sem_init(p3,1,0);
	sem_init(stol_prazan,1,0);

	printf("Pusac 1: ima papir\n");
	printf("Pusac 2: ima šibice\n");
	printf("Pusac 3: ima duhan\n");
	printf("\n");

	pid = fork();
	if(pid == 0){
		trgovac();
		exit(1);
	}
	else if(pid == -1){
		perror("Greska pri stvaranju procesa");
		exit(1);
	}
	pid = fork();
	if(pid == 0){
		pusac(1);
		exit(1);
	}
	else if(pid == -1){
		perror("Greska pri stvaranju procesa");
		exit(1);
	}
	pid = fork();
	if(pid == 0){
		pusac(2);
	}
	else if(pid == -1){
		perror("Greska pri stvaranju procesa");
		exit(1);
	}
	pid = fork();
	if(pid == 0){
		pusac(3);
	}
	else if(pid == -1){
		perror("Greska pri stvaranju procesa");
		exit(1);
	}
	

	sleep(2);

	for (i = 0; i < 4; i++ )
		wait(NULL);

	sem_destroy(KO);
	shmdt(KO);
	sem_destroy(p1);
	shmdt(p1);
	sem_destroy(p2);
	shmdt(p2);
	sem_destroy(p3);
	shmdt(p3);
	sem_destroy(stol_prazan);
	shmdt(stol_prazan);

	return 0;
}