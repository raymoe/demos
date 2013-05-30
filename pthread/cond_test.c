#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

int good_count = 0;
int total = 0;
pthread_mutex_t mt;
pthread_cond_t cv;
volatile int bwait = 1;

void* func1()
{

    pthread_mutex_lock(&mt);
    printf("Producer Wait;\n");
    bwait = 1;
    pthread_cond_wait(&cv, &mt);
    bwait = 0;
    printf("Received a signal\n");
    pthread_mutex_unlock(&mt);

    pthread_exit(NULL);
}

struct timeval now;
struct timespec outtime;

void* func2()
{
    //pthread_mutex_lock(&mt);
    while(bwait)
    {
        pthread_mutex_lock(&mt);
        pthread_cond_signal(&cv);
        //sleep(1);
        gettimeofday(&now, NULL);
        outtime.tv_sec = now.tv_sec + 5;
        outtime.tv_nsec = now.tv_usec * 1000;

        pthread_cond_timedwait(NULL,NULL,&outtime);
        //pth_sleep(1);
        pthread_mutex_unlock(&mt);
    }
    //pthread_cond_signal(&cv);
   // pthread_mutex_unlock(&mt);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[2];
    pthread_attr_t attr;

    /*initialize mutex and cond */
    pthread_mutex_init(&mt, NULL);
    pthread_cond_init(&cv, NULL);

    /*initialize thread attribute*/
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, func2, NULL);
    pthread_create(&threads[1], &attr, func1, NULL);

    int i;
    for (i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mt);
    pthread_cond_destroy(&cv);
    pthread_exit(NULL);
}
