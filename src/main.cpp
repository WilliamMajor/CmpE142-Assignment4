#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void *customer1(void *threadid);
void *customer2(void *threadid);
void *customer3(void *threadid);
void *chef(void *threadid);

int main(int argc, char *argv[]) {
	
	pthread_t p1, p2, p3, p4;
	int i, rc;
	cout << "start of main" << endl;
	rc = pthread_create(&p1, NULL, &customer1, NULL);
	rc = pthread_create(&p2, NULL, &customer2, NULL);
	rc = pthread_create(&p3, NULL, &customer3, NULL );
	rc = pthread_create(&p4, NULL, &chef, NULL );

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	pthread_join(p4,NULL);

	cout << "end of main" << endl;

}
void *customer1(void *threadid)
{
	cout << "customer 1" << endl;
	pthread_exit(NULL);
}
void *customer2(void *threadid)
{
	cout << "customer 2" << endl;
	pthread_exit(NULL);
}
void *customer3(void *threadid)
{
	cout << "customer 3" << endl;
	pthread_exit(NULL);
}
void *chef(void *threadid)
{
	cout << "CHEF!" << endl;
	pthread_exit(NULL);
}
