#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NITER 1000000

//int cnt = 0;
pthread_mutex_t cnt_mutex = PTHREAD_MUTEX_INITIALIZER;
int shmid;
key_t key;
int SHMSZ = sizeof(int);
struct Stuff {
  int* shm_ptr;
};

void * Count(void * stuff)
{
    int i, tmp;
    for(i = 0; i < NITER; i++)
    {
        int* ptr = ((struct Stuff*) stuff)->shm_ptr;
      
        pthread_mutex_lock (&cnt_mutex);
        tmp = *ptr;      /* copy the global cnt locally */
        tmp = tmp+1;    /* increment the local copy */
        *ptr = tmp;      /* store the local value into the global cnt */
        pthread_mutex_unlock (&cnt_mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char * argv[])
{
    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&cnt_mutex, NULL);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        /*
     * We'll name our shared memory segment
     * "5678".
     */
    //int* shm;
    key = IPC_PRIVATE;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_PRIVATE | IPC_CREAT | 0666 )) < 0) {
        perror("shmget");
        exit(1);
    }
    /*
     * Now we attach the segment to our data space.
     */
     int* shm;
    shm = (int *) shmat(shmid, NULL, 0);

    *shm = 0;

    struct Stuff stuffs;
    stuffs.shm_ptr = shm;

    if(pthread_create(&tid1, &attr, Count, &stuffs))
    {
      printf("\n ERROR creating thread 1");
      exit(1);
    }

    if(pthread_create(&tid2, &attr, Count, &stuffs))
    {
      printf("\n ERROR creating thread 2");
      exit(1);
    }
    if(pthread_create(&tid3, &attr, Count, &stuffs))
    {
      printf("\n ERROR creating thread 3");
      exit(1);
    }
    if(pthread_create(&tid4, &attr, Count, &stuffs))
    {
      printf("\n ERROR creating thread 4");
      exit(1);
    }
    if(pthread_create(&tid5, &attr, Count, &stuffs))
    {
      printf("\n ERROR creating thread 5");
      exit(1);
    }

    if(pthread_join(tid1, NULL))	/* wait for the thread 1 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }

    if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }
    if(pthread_join(tid3, NULL))        /* wait for the thread 3 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }
    if(pthread_join(tid4, NULL))        /* wait for the thread 4 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }
    if(pthread_join(tid5, NULL))        /* wait for the thread 5 to finish */
    {
      printf("\n ERROR joining thread");
      exit(1);
    }

    if (*shm < 5 * NITER) 
        printf("\n BOOM! cnt is [%d], should be %d\n", *shm, 5*NITER);
    else
        printf("\n OK! cnt is [%d]\n", *shm);

    shmctl(shmid, IPC_RMID, NULL);
    pthread_exit(NULL);
}

