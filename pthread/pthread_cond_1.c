#include <pthread.h>

int main()
{
    pthread_mutex_t mp;
    pthread_cond_t cv;

    int ret;

    ret = pthread_cond_wait(&cv,&mp);
}
