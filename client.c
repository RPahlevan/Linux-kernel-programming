/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    int shmid;
    key_t key;
    int shmflg;
    int size=10;
    shmflg = IPC_CREAT | 0666;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget (key, size, shmflg)) < 0) {
        perror("shmget");
        exit(1);
    }else
    {
	    (void) fprintf(stderr, "shmget: client shmget returned     %d\n", shmid);
	   
	    }

    /*
     * Now we attach the segment to our data space.
     */
     char* shm;
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Now read what the server put in the memory.
     */
     char *p;
     p=shm;
     printf("\nMessegae in the shared memeroy: %s\n",p);

    /*Detach from the shared memory*/
    int retval;
    if ((retval=shmdt(p))<0)
    {
	    perror("\nserver shmdt error\n");
	    exit(1);
	    }
	    

	    printf("\nclient program finished succesfully!\n");
	    return 0;

}

