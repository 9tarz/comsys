#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//#define NITER 1000000
#define NITER 10000

//int cnt = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shmid;
key_t key;
int SHMSZ = sizeof(int);
struct Stuff {
  int* shm_ptr;
  int tid;
};

enum { STATE_A, STATE_B, STATE_C, STATE_D } state = STATE_A;
pthread_cond_t      condA  = PTHREAD_COND_INITIALIZER;
pthread_cond_t      condB  = PTHREAD_COND_INITIALIZER;
pthread_cond_t      condC  = PTHREAD_COND_INITIALIZER;
pthread_cond_t      condD  = PTHREAD_COND_INITIALIZER;


void* CountA(void * stuff)
{
    int i, tmp, j;
    int* ptr = ((struct Stuff*) stuff)->shm_ptr;
    int tid = ((struct Stuff*) stuff)->tid;
    for(i = 0 ; i < (NITER)/5; i++)
    {
    pthread_mutex_lock(&mutex);
      while (state != STATE_A)
        pthread_cond_wait(&condA, &mutex);
    pthread_mutex_unlock(&mutex);

      for(j = 0; j < 5; j++)
        {
          pthread_mutex_lock (&mutex);
          tmp = *ptr;      /* copy the global cnt locally */
          tmp = tmp+1;    /* increment the local copy */
          *ptr = tmp;      /* store the local value into the global cnt */
          pthread_mutex_unlock (&mutex);
        }
        pthread_mutex_lock(&mutex);
        state = STATE_B;
        pthread_cond_signal(&condB);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
void* CountB(void * stuff)
{
    int i, tmp, j;
    int* ptr = ((struct Stuff*) stuff)->shm_ptr;
    int tid = ((struct Stuff*) stuff)->tid;
    for(i = 0 ; i < (NITER)/5; i++)
    {
    pthread_mutex_lock(&mutex);
      while (state != STATE_B)
        pthread_cond_wait(&condB, &mutex);
    pthread_mutex_unlock(&mutex);

      for(j = 0; j < 5; j++)
        {
          pthread_mutex_lock (&mutex);
          tmp = *ptr;      /* copy the global cnt locally */
          tmp = tmp+1;    /* increment the local copy */
          *ptr = tmp;      /* store the local value into the global cnt */
          pthread_mutex_unlock (&mutex);
        }
        pthread_mutex_lock(&mutex);
        state = STATE_C;
        pthread_cond_signal(&condC);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
void* CountC(void * stuff)
{
    int i, tmp, j;
    int* ptr = ((struct Stuff*) stuff)->shm_ptr;
    int tid = ((struct Stuff*) stuff)->tid;
    for(i = 0 ; i < (NITER)/5; i++)
    {
    pthread_mutex_lock(&mutex);
      while (state != STATE_C)
        pthread_cond_wait(&condC, &mutex);
    pthread_mutex_unlock(&mutex);

      for(j = 0; j < 5; j++)
        {
          pthread_mutex_lock (&mutex);
          tmp = *ptr;      /* copy the global cnt locally */
          tmp = tmp+1;    /* increment the local copy */
          *ptr = tmp;      /* store the local value into the global cnt */
          pthread_mutex_unlock (&mutex);
        }
        pthread_mutex_lock(&mutex);
        state = STATE_D;
        pthread_cond_signal(&condD);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
void* CountD(void * stuff)
{
    int i, tmp, j;
    int* ptr = ((struct Stuff*) stuff)->shm_ptr;
    int tid = ((struct Stuff*) stuff)->tid;
    for(i = 0 ; i < (NITER)/5; i++)
    {
    pthread_mutex_lock(&mutex);
      while (state != STATE_D)
        pthread_cond_wait(&condD, &mutex);
    pthread_mutex_unlock(&mutex);

      for(j = 0; j < 5; j++)
        {
          pthread_mutex_lock (&mutex);
          tmp = *ptr;      /* copy the global cnt locally */
          tmp = tmp+1;    /* increment the local copy */
          *ptr = tmp;      /* store the local value into the global cnt */
          pthread_mutex_unlock (&mutex);
        }

        pthread_mutex_lock(&mutex);
        state = STATE_A;
        pthread_cond_signal(&condA);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


int main(int argc, char * argv[])
{
    pthread_t tid1, tid2, tid3, tid4;

    //pthread_t threads[4];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);
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

    pthread_create(&tid1, &attr, CountA, &stuffs);
    pthread_create(&tid2, &attr, CountB, &stuffs);
    pthread_create(&tid3, &attr, CountC, &stuffs);
    pthread_create(&tid4, &attr, CountD, &stuffs);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    if (*shm < 4 * NITER) 
        printf("\n BOOM! cnt is [%d], should be %d\n", *shm, 4*NITER);
    else
        printf("\n OK! cnt is [%d]\n", *shm);

    shmctl(shmid, IPC_RMID, NULL);
    pthread_exit(NULL);
}

