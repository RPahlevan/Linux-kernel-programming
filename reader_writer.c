/*This program is written by Ramin Pahlevannejad*/

/*This program is created for POSIX*/
/*This program is write for readers/writers application with given a priority to reader*/
/*Multithread programming on Linux kernel*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5 // number of readers and writers
int X=5;// a variable that make the reader or writer to read from buffer
int buffer; // global variable buffer for both readers and writers
int Resource_Counter=0;// demonstrate the number of reader thread
int Waiting_Readers=0;// number of waiting readers
  
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  	/* mutex lock for buffer */
pthread_cond_t c_read = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_write = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

//function prototype
void *reader (void *param);
void *writer (void *param);

int main(int argc, char *argv[]) {

	pthread_t tid[NUM_THREADS];  /* thread identifiers */
	int i;//index for for loop
	int tNum[NUM_THREADS];// an array to hold the thread number for readers
	int tNum1[NUM_THREADS];//an array to hold a threads number for writer

	/* create the threads; may be any number, in general */
	/* create a reader threads*/
	for (i=0;i<NUM_THREADS;i++){
		tNum[i] = i;
		if(pthread_create(&tid[i], NULL, reader, &tNum[i]) != 0) {
			fprintf(stderr, "Unable to create producer thread\n");
			exit(1);
		}
	}

	/*create a writer threads*/
	for(i=0;i<NUM_THREADS;i++){
		tNum1[i]=i;
		if(pthread_create(&tid[i], NULL, writer, &tNum1[i]) != 0) {
			fprintf(stderr, "Unable to create consumer thread\n");
			exit(1);
			}
	}

	/* wait for created thread to exit */
	for(i = 0; i < NUM_THREADS; i++) { /* wait/join threads */
		pthread_join(tid[i], NULL);
	}
	printf("Parent quiting\n");

	return 0;
}

void* reader(void* param)
{
	int Reader_Counter = *((int*)param);
	printf("Reader thread number %d\n", Reader_Counter);
	int i=0;// for loop oindex value
	int Num_Readers=0;// number of readers

	for (i=0;i<X;i++)
	{
		pthread_mutex_lock(&m);
		Waiting_Readers++;//as soon reader thread creates, this variable will be increased
		// over folw of reader threads
		if (Reader_Counter>5)
		{
			printf("\nerror, reader threads overfolw!\n");
			exit(1);
		}
		// witer thread is modifying the buffer, so reader threads has to be wait for the signal from reader
		while (Resource_Counter==-1)
		{
			pthread_cond_wait (&c_read, &m);
		}
		Waiting_Readers--;//one reader pass the lock
		Resource_Counter++;// one thread pass the lock, so the resource counter has to be added
		Num_Readers=Resource_Counter;
		pthread_mutex_unlock(&m);
		//read the value from shared global variable
		
		// Read data
		fprintf(stdout, "[Reaader Thread id: %d] reading value %u  [Waiting readers threads: %2d]\n", Reader_Counter, buffer,Num_Readers);

		pthread_mutex_lock(&m);
		Resource_Counter--;// after finishing a read phase, count down a resource counter
		if(Resource_Counter==0)// there is not any created reader thread.
		{
			pthread_cond_signal (&c_write);//switch to write phase if it's needed
		}
		pthread_mutex_unlock(&m);  
	}
	pthread_exit(0);
}


void* writer(void* param)
{
	int Writer_Counter = *((int*)param);// type cast void* to int* in order to get the thread number
	printf("Writer thread number %d\n", Writer_Counter);
	int i=0;
	for(i=0;i<X;i++){
		pthread_mutex_lock(&m);// lock other threads to modify the variable
		// writer threads overflow
		if(Writer_Counter>5)
		{
			exit(0);
		}

		while(Resource_Counter!=0)
		{
			pthread_cond_wait (&c_write, &m);
		}
		Resource_Counter = -1;
	  	int Num_Readers = Resource_Counter;
	  pthread_mutex_unlock(&m);

	  // Write data
		buffer++;
		fprintf(stdout, "[writer Thread id: %d] write value %u  [Waiting readers threads: %2d]\n", Writer_Counter,buffer,Num_Readers);
		pthread_mutex_lock(&m);
		Resource_Counter=0;// update the resource counter
		// if any readers threads wait, they will have the proiority
		//else, only wake up one writer thread
		if (Waiting_Readers > 0) {
	  		pthread_cond_broadcast(&c_read);
	  	}
	  	else {
	  		pthread_cond_signal(&c_write);
	  	}
		pthread_mutex_unlock(&m);// unlock other threads
	}
	pthread_exit(0);
}
