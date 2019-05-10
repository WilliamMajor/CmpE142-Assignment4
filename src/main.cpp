#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t lock     	= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t foodOut   	= PTHREAD_COND_INITIALIZER;

void *customerH(void * A);
void *customerF(void * A);
void *customerS(void * A);
void *chef(void * A);

bool chefSleep;
int happyCustomer = 0;
//use an int array to handle the current food being served 
int food[3] = {0,0,0}; //i[0] = Handburger, i[1] = Fries, i[2] = Soda

int main(int argc, char *argv[]) {
	
	pthread_t c1, c2, c3, chef1;
    
    int rc;
	//cout << "start of main" << endl;

	rc = pthread_create(&chef1, NULL, &chef, NULL);
	rc = pthread_create(&c1, NULL, &customerH, NULL);
	//rc = pthread_create(&p2, NULL, &customerF, NULL);
	//rc = pthread_create(&p3, NULL, &customerS, NULL);
	


	pthread_join(c1,NULL);
	//pthread_join(p2,NULL);
	//pthread_join(p3,NULL);
	pthread_join(chef1,NULL);

	//cout << "end of main" << endl;

}
void *customerH(void * A)
{
	int count = 0;
	int mealcount = 0;
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = false;
	while (count <100)
	{
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&foodOut, &lock);
		
		//cout << food[0] << "," << food[1] << "," << food[2] << endl << endl;
		(food[1] == 1) ? hasFries = true : hasFries = false;
		(food[2] == 1) ? hasSoda = true : hasSoda = false;

		if(hasHamburger && hasFries && hasSoda)
		{
			mealcount++;
		}
		pthread_mutex_unlock(&lock);
		hasFries = false;
		hasSoda = false;
		count++;
	}
	cout << "Customer 1 meals eaten " << mealcount << endl;
	pthread_exit(NULL);
}
void *customerF(void * A)
{
    bool hasHamburger = false;
    bool hasFries = true;
    bool hasSoda = false;
	//cout << "customer 2" << endl;
	pthread_exit(NULL);
}
void *customerS(void * A)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = true;
	//cout << "customer 3" << endl;
	pthread_exit(NULL);
}
// we need to randomly generate food then fall alseep waiting for a wakup flag
void *chef(void * A)
{
	int f1,f2;
	int count = 0;
	srand(time(NULL));
	while (count <= 100)
	{
		pthread_mutex_lock(&lock);
		food[0] = 0;
		food[1] = 0;
		food[2] = 0;

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
		food[f1] = 1;
		food[f2] = 1;
		
		//cout << food[0] << "," << food[1] << "," << food[2] << endl << endl;
		pthread_cond_signal(&foodOut);
		
		pthread_mutex_unlock(&lock);
		usleep(100);
		count++;
	}

	
	pthread_exit(NULL);
}
