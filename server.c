#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h> 

int main (int arc, char* argv[])
{

	
	/*create a sherd memory*/
	//The key argument is a access value associated with the semaphore ID.
	key_t key; /* key to be passed to shmget() */

	//The shmflg argument specifies the initial access permissions and creation control flags.
	int shmflg; /* shmflg to be passed to shmget() */ 
	
	//When the call succeeds, it returns the shared memory segment ID. This call is also used to get the ID of an existing shared segment
	int shmid; /* return value from shmget() */

	//The size argument is the size in bytes of the requested shared mm  
	int size; /* size to be passed to shmget() */
	 
	key =5678;//unique key 
	size = 10; // define the size of the buffer 
	shmflg = IPC_CREAT | 0666;//0666 sets the access permissions of the memory segment while IPC_CREAT tells the system to create a new memory segment for the shared memory. 
	if ((shmid = shmget (key, size, shmflg)) == -1) {
		perror("\n servershmget: shmget failed\n"); 
		exit(1); 
		} 
		else 
		{
			(void) fprintf(stderr, "shmget: server shmget returned %d\n", shmid);
		}
		/*attach to the created memory*/
		//linux format: void *shmat(int shmid, const void *shmaddr, int shmflg);
		   char* shm;
		   if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
			   perror("\n server shmat error\n");
			   exit(1);
			   }
	/*write into the created buffer and put it into the shared memory*/
	char *p;
	p=shm;//use p as a pointer to the shared memory buffer

	memset(p,'\0',10);// cleaning a buffer before use
	memcpy(p,"Ramin joon!",10);//copy Ramin into the shared memory


	/* detach from the shared memory*/
	//original format: int shmdt(const void *shmaddr);
	int retval;
	if ((retval=shmdt(p))<0)
	{
		perror("\nserver shmdt error\n");
		exit(1);
	}
	printf("\nserver program finished succesfully!\n");

	return 0;
}
