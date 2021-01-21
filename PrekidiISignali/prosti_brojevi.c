#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>


int prost ( unsigned long n ) {
	unsigned long i, max;

	if ( ( n & 1 ) == 0 ) 
		return 0;

	max = sqrt ( n );
	for ( i = 3; i <= max; i += 2 )
		if ( ( n % i ) == 0 )
			return 0;

	return 1; 
}

static int pauza = 0;
static long int broj = 1000000001;
static long int zadnji = 1000000001;

void periodicki_ispis (int sig) {
   printf("%s %ld\n" ,"zadnji prosti broj = ",zadnji );
}

void postavi_pauzu (int sig) {
   if(pauza==0){
   	printf("%s\n","Pauziram broj");
   }
   else{
   	printf("%s\n","Vracam nazad");
   }
   pauza = 1 - pauza;
}

void prekini (int sig) {
   printf("%s %ld\n" ,"ZADNJI = ",zadnji );
   exit(0);
}


int main ()
{
	struct itimerval t;

	
	sigset (SIGALRM, periodicki_ispis);

	sigset (SIGTERM,prekini);
	
	sigset (SIGINT , postavi_pauzu);
	
	

	
	t.it_value.tv_sec = 5;
	t.it_value.tv_usec = 0;
	
	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;

	
	setitimer ( ITIMER_REAL, &t, NULL );

	while(1) {
		if(prost(broj)){
			zadnji = broj;
		}
		broj++;
		while(pauza == 1){
			pause();
		}
	}

	return 0;
}
