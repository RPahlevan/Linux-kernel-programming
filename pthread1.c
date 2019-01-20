#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUFF_SIZE 5

// mutex and condition variable initialization
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;  	/* mutex lock for buffer */
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

// global variable for buffer control
int arr[BUFF_SIZE];
int add=-1;// an index for adding an element
int num=0;// shows the number of elements are availble inside the shared buffer
int rem=-1;// an index for removing an element from buffer

// function prototype
void* producer(void* );
void* consumer(void* );


int main(int argc, char * argv[])
{
	pthread_t tid1;// producer thread
	pthread_t tid2;// consumer thread
	
	// create thread for producer
	if (pthread_create(&tid1, NULL,producer, NULL)!=0)// tid1 as thread for producer, attr set as default by NULL, start_routine==producer, void* arg==NULL or default
	{
		printf("\nerror in making producer consumer!\n");
		exit(1);
	}

	// create a thread for consumer
	if (pthread_create(&tid2, NULL,consumer, NULL)!=0)
        {
                printf("\nerror in making consumer consumer!\n");
		exit(1);
        }

	// use pthread join to make sure all thread will be terminated
	if (pthread_join(tid1, NULL)!=0)
	{
		printf("\n pthread join for producer error\n");
		exit(1);
	}
	
	if (pthread_join(tid2, NULL)!=0)
        {
                printf("\n pthread join for consumer error\n");
		exit(1);
        }

	return 0;
}

// producer funtion
void* producer(void* param)
{
	int i=0; // for loop index
	for(i=0;i<=20;i++)
	{
		pthread_mutex_lock(&m);//using mutex to lock the buffer to enter the value inside the buffer
		if (num > BUFF_SIZE) {
			exit(1);  /* overflow */
		}

		while(num==BUFF_SIZE)
		{
			pthread_cond_wait (&c_prod, &m);// if the buffer is full, wait for the consumer signal
		}
		add=(add+1)%BUFF_SIZE;// circular array buffer
		arr[add]=i;
		num++;// after adding a buffer, num has to be added so we know the number of elements inside the buffer
		
		pthread_mutex_unlock(&m);// after inserting an element, now we can unlock the buffer
		pthread_cond_signal (&c_cons);// send a signal to consumer thread inorder to wake up and use the data if it's needed
		printf ("producer: inserted %d\n", i);
		fflush (stdout);// force the printf to output stream
	}

	// this function will be terminated after 21 times producing
	printf("producer quiting\n");
	fflush(stdout);
	return 0;
}

// consumer funtion
void* consumer(void* param)
  {        
	  int i=0; // for loop index
	  for(i=0;i<=20;i++)
	  {
                  pthread_mutex_lock(&m);//using mutex to lock the buffer to enter the value inside the buffer
		  if (num < 0) {
			  exit(1); 
			  } /* underflow */
		  while (num==0)
		  {
			  pthread_cond_wait(&c_cons,&m);

		}
		rem=(rem+1)%BUFF_SIZE;
		//i=arr[rem];
		num--;
   
                  pthread_mutex_unlock(&m);// after inserting an element, now we can unlock the buffer
                  pthread_cond_signal (&c_prod);// send a signal to producer thread inorder to wake up and use the data if it's needed
                  printf ("consume: value  %d\n", i);
                  fflush (stdout);// force the printf to output stream
          }
	  // this function will be terminated after 21 times producing
          printf("consumer quiting\n");
          fflush(stdout);
          return 0;
  }


