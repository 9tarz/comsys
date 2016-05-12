#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <queue>

//#define NITER 1000000
#define NITER 10000
using namespace std;

//int cnt = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shmid;
key_t key;
int SHMSZ = sizeof(int);
struct Stuff {
  int* shm_ptr;
  int tid;
};

int count_thread = 0;
queue <int> q;

void* Count(void * stuff)
{
    int i, tmp;
    int* ptr = ((struct Stuff*) stuff)->shm_ptr;
    int tid = ((struct Stuff*) stuff)->tid;

    if (*ptr >= 4*NITER)
      pthread_exit(NULL);
    for(i = 0; i < 5; i++)
    {
        pthread_mutex_lock (&mutex);
        tmp = *ptr;      /* copy the global cnt locally */
        tmp = tmp+1;    /* increment the local copy */
        *ptr = tmp;      /* store the local value into the global cnt */
        pthread_mutex_unlock (&mutex);
    }
    pthread_exit(NULL);
}


int main(int argc, char * argv[])
{
    //pthread_t tid1, tid2, tid3, tid4, tid5;

    pthread_t threads[4];

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

    for(int i = 0 ; i < 4 ;i++)
        q.push(i);

    while(*shm < 4*NITER) {
        if (!q.empty()) {
          int pop_q = q.front();
          q.pop();
          stuffs.tid = pop_q;
          pthread_create(&threads[pop_q], NULL, &Count, &stuffs);
          //printf("[tid:%d] %d\n", pop_q, *shm);
          for(int i = 0 ; i < 4 ;i++)
            q.push(i);
        }
      }
    /*pthread_create(&tid1, &attr, Count, &stuffs);
    pthread_create(&tid2, &attr, Count, &stuffs);
    pthread_create(&tid3, &attr, Count, &stuffs);
    pthread_create(&tid4, &attr, Count, &stuffs);*/

    /*pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);*/

    if (*shm < 4 * NITER) 
        printf("\n BOOM! cnt is [%d], should be %d\n", *shm, 5*NITER);
    else
        printf("\n OK! cnt is [%d]\n", *shm);

    shmctl(shmid, IPC_RMID, NULL);
    pthread_exit(NULL);
}

