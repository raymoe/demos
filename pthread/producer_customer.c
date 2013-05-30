#include <pthread.h>
#include <stdio.h>

int good_count = 0;
int total = 0;
pthread_mutex_t mt;
pthread_cond_t cv;
int producer_wait;

void* produce()
{
    while(total < 100)
    {
        pthread_mutex_lock(&mt);
        good_count += 5;
        printf("Producer produce 5 goods;\n");
        producer_wait = 1;
        pthread_cond_wait(&cv, &mt);
        producer_wait = 0;
        printf("Received a signal\n");
        pthread_mutex_unlock(&mt);
    }
    pthread_exit(NULL);
}

void* consume()
{
    while (total < 100) {
        pthread_mutex_lock(&mt);
        if(good_count <= 0)
        {
            printf("the good_count is 0,notify producer to produce;\n");
            int ret = pthread_cond_signal(&cv);
            //sleep(1);
            //pthread_cond_wait(&cv,&mt);
        }
        else
        {
            good_count -= 1;
            total += 1;
            printf("Consumer consume 1 good,consume totol=%d;\n",total);
        }
        pthread_mutex_unlock(&mt);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[2];
    pthread_attr_t attr;

    /*初始化互斥量和条件变量对象*/
    pthread_mutex_init(&mt, NULL);
    pthread_cond_init(&cv, NULL);

    /*创建线程时设为可连接状态，便于移植*/
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, produce, NULL);
    pthread_create(&threads[1], &attr, consume, NULL);

    int i;
    for (i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mt);
    pthread_cond_destroy(&cv);
    pthread_exit(NULL);
}
