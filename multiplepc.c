#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>

/* define buffer size */
#define SIZE 5

/* global variables declaration */
int buffer[SIZE];
int w_poz = 0, r_poz = 0, n_p = 0;

/* data to be written by producers */
const int written_data[5] = {1, 2, 5, 7, 9};

/* semaphores and mutex declaration */
sem_t buffer_full, buffer_empty;
pthread_mutex_t mutex;

void write_in_buffer(int value)
{
	buffer[w_poz] = value;
	w_poz = (w_poz + 1) % SIZE;
	printf("wrote: %d \n",value);
}

int read_from_buffer()
{
	int value = buffer[r_poz];
	r_poz = (r_poz + 1) % SIZE;
	return value;
}

void *prod(void *arg)
{
	int i = 0;

	while(1)
	{
		/* decrement and wait for the semaphore to signal 
		that buffer is empty*/
		sem_wait(&buffer_empty);

		i = i %SIZE;

		/* lock the mutex */
		pthread_mutex_lock(&mutex);

		printf("Producer:%p, ", arg);

		write_in_buffer(written_data[i]);

		/* increment index value for the next writing */
		++i;
		
		/* check the semaphore value */
		sem_getvalue(&buffer_full, &n_p);
		printf("Buffer_full semaphore value is :%d \n", n_p);

		/* unlock the mutex */
		pthread_mutex_unlock(&mutex);

		/* increment the buffer full semaphore */
		sem_post(&buffer_full);
	}
}

void *cons(void *arg)
{
	while(1)
	{
		/* decrement and wait for the semaphore to signal 
		that buffer is full*/
		sem_wait(&buffer_full);
		
		/* Lock the mutex */
		pthread_mutex_lock(&mutex);

		printf("Read data by consumer %p is: %d \n", 
												 arg, read_from_buffer());

		/* Unlock the mutex */
		pthread_mutex_unlock(&mutex);

		/* increment the buffer empty semaphore */
		sem_post(&buffer_empty);
	}
}

int main(void)
{
	/* declaration of producer and consumer threads */
	pthread_t producer_th, producer_th_2,
								consumer_th, consumer_th_3, consumer_th_5;

	/* semaphores initialisation */
	sem_init(&buffer_empty, 0, SIZE);
	sem_init(&buffer_full, 0, 0); 

	/* buffer initialization */
	memset(buffer,0,sizeof(int)*SIZE);

	/* create producer threads */
	pthread_create(&producer_th, NULL, prod, (void *)2);
	pthread_create(&producer_th_2, NULL, prod, (void *)4);
	//pthread_create(&producer_th_4, NULL, prod, (int*)6);
	//pthread_create(&producer_th_6, NULL, prod, (int*)8);

	/* create consumer threads */
	pthread_create(&consumer_th, NULL, cons, (void *)3); 
	pthread_create(&consumer_th_3, NULL, cons, (void *)5); 
	pthread_create(&consumer_th_5, NULL, cons, (void *)7); 

	/* wait for producer threads to finish */
	pthread_join(producer_th, NULL);
	pthread_join(producer_th_2, NULL);
	//pthread_join(producer_th_4, NULL);
	//pthread_join(producer_th_6, NULL);

	/* wait for consumer threads to finish */
	pthread_join(consumer_th, NULL);
	pthread_join(consumer_th_3, NULL);
	pthread_join(consumer_th_5, NULL);

	return 0;
}
