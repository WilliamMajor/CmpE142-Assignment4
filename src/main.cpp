#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <pthread.h>

using namespace std;

pthread_mutex_t lock     	= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock1     	= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2     	= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3     	= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t foodOut1   	= PTHREAD_COND_INITIALIZER;
pthread_cond_t foodOut2		= PTHREAD_COND_INITIALIZER;
pthread_cond_t foodOut3		= PTHREAD_COND_INITIALIZER;
pthread_cond_t C2Done		= PTHREAD_COND_INITIALIZER;
//pthread_cond_t C3Done		= PTHREAD_COND_INITIALIZER;

void *customerH(void * A);
void *customerF(void * A);
void *customerS(void * A);
void *chef(void * A);

//use an int array to handle the current food being served 
int food[3] = {0,0,0}; //i[0] = Handburger, i[1] = Fries, i[2] = Soda

//volatile bool foodReady = false;
atomic <bool> done1(false);
atomic <bool> done2(false);
atomic <bool> done3(false);
volatile bool cdone = false;
int mealcountc1 = 0;
int mealcountc2 = 0;
int mealcountc3 = 0;

int main(int argc, char *argv[]) {
	
	pthread_t c1, c2, c3, chef1;
    
    int rc;
	//cout << "start of main" << endl;

	
	rc = pthread_create(&c1, NULL, &customerH, NULL);
	rc = pthread_create(&c2, NULL, &customerF, NULL);
	rc = pthread_create(&c3, NULL, &customerS, NULL);
	rc = pthread_create(&chef1, NULL, &chef, NULL);
	


	pthread_join(c1,NULL);
	pthread_join(c2,NULL);
	pthread_join(c3,NULL);
	pthread_join(chef1,NULL);

	//printf("Customer 1 meals eaten %d\n",mealcountc1);
	//printf("Customer 2 meals eaten %d\n",mealcountc2);
	cout << "Customer 1 meals eaten " << mealcountc1 << endl;
	cout << "Customer 2 meals eaten " << mealcountc2 << endl;
	cout << "Customer 3 meals eaten " << mealcountc3 << endl;
	//cout << "end of main" << endl;

}
void *customerH(void * A)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = false;
	while (!cdone)
	{
		//pthread_mutex_lock(&lock);
		pthread_cond_wait(&foodOut1, &lock1);
		pthread_mutex_unlock(&lock1);
		//cout << food[0] << "," << food[1] << "," << food[2] << endl << endl;
		(food[1] == 1) ? hasFries = true : hasFries = false;
		(food[2] == 1) ? hasSoda = true : hasSoda = false;
		if(hasHamburger && hasFries && hasSoda)
		{
			mealcountc1++;
		}	
		hasFries = false;
		hasSoda = false;
		pthread_mutex_lock(&lock);
		done1 = true;
		pthread_mutex_unlock(&lock);
	}
	//cout << "Customer 1 meals eaten " << mealcountc1 << endl;
	pthread_exit(NULL);
}

void *customerF(void * A)
{
    bool hasHamburger = false;
    bool hasFries = true;
    bool hasSoda = false;
	while (!cdone)
	{
		//cout << "hello1" << endl;
		//pthread_mutex_lock(&lock);
		pthread_cond_wait(&foodOut2, &lock2);
		pthread_mutex_unlock(&lock2);
		//cout << food[0] << "," << food[1] << "," << food[2] << endl << endl;
		(food[0] == 1) ? hasHamburger = true : hasHamburger = false;
		(food[2] == 1) ? hasSoda = true : hasSoda = false;
		if(hasHamburger && hasFries && hasSoda)
		{
			mealcountc2++;
		}
		hasHamburger = false;
		hasSoda = false;
		pthread_mutex_lock(&lock);
		done2 = true;
		pthread_mutex_unlock(&lock);	
	}
	pthread_exit(NULL);
}
void *customerS(void * A)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = true;
	while (!cdone)
	{
		//pthread_mutex_lock(&lock);
		pthread_cond_wait(&foodOut3, &lock3);
		pthread_mutex_unlock(&lock3);
		
		//cout << food[0] << "," << food[1] << "," << food[2] << endl << endl;
		(food[0] == 1) ? hasHamburger = true : hasHamburger = false;
		(food[1] == 1) ? hasFries = true : hasFries = false;

		if(hasHamburger && hasFries && hasSoda)
		{
			mealcountc3++;
		}
		hasHamburger = false;
		hasFries = false;
		pthread_mutex_lock(&lock);
		done3 = true;
		pthread_mutex_unlock(&lock);
		
	}
	//cout << "customer 3" << endl;
	pthread_exit(NULL);
}
// we need to randomly generate food then fall alseep waiting for a wakup flag
void *chef(void * A)
{
	int f1,f2;
	int count = 0;
	srand(time(NULL));
	while (count < 99)
	{
		pthread_mutex_lock(&lock);
		food[0] = 0;
		food[1] = 0;
		food[2] = 0;
		pthread_mutex_unlock(&lock);

		f1 = rand() % 3000;
		f2 = rand() % 3000;

		//cout << "count" << count << endl;

		if(f1 < 1000)
		{
			f1 = 0;
		}
		else (f1 > 2000) ? f1 = 2 : f1 = 1;

		switch (f1)
		{
			case 0:
				(f2 < 1500) ? f2 = 1 : f2 = 2;
				break;
			case 1:
				(f2 < 1500) ? f2 = 0 : f2 = 2;
				break;
			case 2:
				(f2 < 1500) ? f2 = 0: f2 = 1;
				break;
		}
		pthread_mutex_lock(&lock);
		food[f1] = 1;
		food[f2] = 1;
		//foodReady = true;
		pthread_cond_signal(&foodOut1);
		pthread_cond_signal(&foodOut2);
		pthread_cond_signal(&foodOut3);
		pthread_mutex_unlock(&lock);
		
		//cout << food[0] << "," << food[1] << "," << food[2] << endl << endl;
		while(!done1.load() || !done2.load() || !done3.load())
		{
			usleep(1000);
		}
		
		
		pthread_mutex_lock(&lock);
		done1 = false;
		done2 = false;
		done3 = false;
		pthread_mutex_unlock(&lock);
		count++;
	}
	pthread_mutex_lock(&lock);
	cdone = true;
	pthread_mutex_unlock(&lock);
	pthread_cond_signal(&foodOut1);
	pthread_cond_signal(&foodOut2);
	pthread_cond_signal(&foodOut3);

	
	pthread_exit(NULL);
}
