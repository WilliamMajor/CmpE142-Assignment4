#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void *customerH(void *threadid);
void *customerF(void *threadid);
void *customerS(void *threadid);
void *chef(void *threadid);

int main(int argc, char *argv[]) {
	
	pthread_t p1, p2, p3, p4;
    //use an int array to handle the current food being served 
	int i[2];
    int rc;
	cout << "start of main" << endl;
	rc = pthread_create(&p1, NULL, &customer1, (void *)i);
	rc = pthread_create(&p2, NULL, &customer2, (void *)i);
	rc = pthread_create(&p3, NULL, &customer3, (void *)i);
	rc = pthread_create(&p4, NULL, &chef, (void *)i);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	pthread_join(p4,NULL);

	cout << "end of main" << endl;

}
void *customerH(void *threadid)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = false;
	cout << "customer 1" << endl;
	pthread_exit(NULL);
}
void *customerF(void *threadid)
{
    bool hasHamburger = false;
    bool hasFries = true;
    bool hasSoda = false;
	cout << "customer 2" << endl;
	pthread_exit(NULL);
}
void *customerS(void *threadid)
{
    bool hasHamburger = true;
    bool hasFries = false;
    bool hasSoda = true;
	cout << "customer 3" << endl;
	pthread_exit(NULL);
}
void *chef(void *threadid)
{
	cout << "CHEF!" << endl;
	pthread_exit(NULL);
}
