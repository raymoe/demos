/**********************************************
 * Author: lewiyon@hotmail.com
 * File name: producer_sonsumer.h
 * Description: define some functions about the module
 *              of producer and consumer
 * Date: 2012-02-12
 *********************************************/



#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

/* The num. of buffers */
#define BUFFER_SIZE 16
#define OVER (- 1)
/* buffer structure */
struct prodcons
{
    int *buffer;    /* the array store data */
    pthread_mutex_t lock;   /*  linux thread mutex */
    int readpos;        /* the pointer of consumer */
    int writepos;       /* the pointer of producer*/
    /* linux thread waiting variable : the buf is not empty */
    pthread_cond_t notempty;
    /* linux thread waiting variable : the buf is not full */
    pthread_cond_t notfull;
};

struct prodcons buffer;

/**
 * prodcons_init - buffer structure initialization
 * @buf: the buf.
 */
void prodcons_init(struct prodcons *buf)
{
    pthread_mutex_init(&buf->lock, NULL);
    pthread_cond_init(&buf->notempty, NULL);
    pthread_cond_init(&buf->notfull, NULL);
    buf->buffer = (int *)malloc(sizeof(int) * BUFFER_SIZE);
    buf->readpos = 0;
    buf->writepos = 0;
}

/**
 * put_dataunit - put the unit of @data into the @buf
 * @buf: the buf.
 * @data: the unit of data.
 */
void put_dataunit(struct prodcons *buf, int data)
{
    pthread_mutex_lock(&buf->lock);
    /* If the buffer is full, and producer will wait for a buf */
    if ((buf->writepos + 1) % BUFFER_SIZE == buf->readpos)
    {
        pthread_cond_wait(&buf->notfull, &buf->lock);
    }

    /* producer gets data,and move the producer's pointer*/
    buf->buffer[buf->writepos] = data;
    buf->writepos++;
    if (buf->writepos >= BUFFER_SIZE)
        buf->writepos = 0;

    /* if the buffer is not empty, set the condition */
    pthread_cond_signal(&buf->notempty);
    pthread_mutex_unlock(&buf->lock);
}

/**
 * get_dataunit - get an unit of @data from the @buf
 * @buf: the buf.
 * retrun
 *       the index of the data unit.
 */
int get_dataunit(struct prodcons *buf)
{
    int data;
    pthread_mutex_lock(&buf->lock);

    /* If the buffer is empty, and consumer will wait for a data */
    if (buf->writepos == buf->readpos)
    {
        pthread_cond_wait(&buf->notempty, &buf->lock);
    }

    /* consumer gets data,and move the consumer's pointer*/
    data = buf->buffer[buf->readpos];
    buf->readpos++;
    if (buf->readpos >= BUFFER_SIZE)
        buf->readpos = 0;

    /* if the buffer is not full, set the condition */
    pthread_cond_signal(&buf->notfull);
    pthread_mutex_unlock(&buf->lock);
    return data;
}



void *producer(void *data)

{

    int n;

    for (n = 0; n < 10000; n++)

    {

        printf("%d --->\n", n);

        put_dataunit(&buffer, n);

    } put_dataunit(&buffer, OVER);

    return NULL;

}



/**

 * consumer - consumer's method

 * @data: the unit of data which will be get from the buffer .

 */

void *consumer(void *data)

{

    int d;

    while (1)

    {

        d = get_dataunit(&buffer);

        if (d == OVER)

            break;

        printf("--->%d \n", d);

    }

    return NULL;

}



int main(int argc, char *argv[])

{

    pthread_t th_a, th_b;

    void *retval;

    prodcons_init(&buffer);



    /* create producer and consumer thread */

    pthread_create(&th_a, NULL, producer, 0);

    pthread_create(&th_b, NULL, consumer, 0);



    /* wait the end of the two threads */

    pthread_join(th_a, &retval);

    pthread_join(th_b, &retval);



    return 0;

}
