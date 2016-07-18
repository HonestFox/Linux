
/*************************************************************************
	> File Name: my_senc.c
	> Author: HonestFox
	> Mail: zhweizhi@foxmail.com 
	> Created Time: Mon 18 Jul 2016 09:04:20 PM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t space_nums;
sem_t data_nums;

pthread_mutex_t consuming = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t producting= PTHREAD_MUTEX_INITIALIZER;


#define _SIZE 6

int buf[_SIZE];
void *consumer(void *arg)
{
	int index = 0;
	while(1)
	{
		sleep(1);
		sem_wait(&data_nums);
		pthread_mutex_lock(&consuming);
		int data = buf[index];
		printf("NO.%ud consumer done... data is : %d, index = %d\n", pthread_self(), data, index);
		sem_post(&space_nums);
		index++;
		index %= _SIZE;
		pthread_mutex_unlock(&consuming);
	}
	printf("\n");
}

void *productor(void *arg)
{
	int index = 0;
	while(1)
	{
		pthread_mutex_lock(&producting);
		sem_wait(&space_nums);
		buf[index] = rand() % 1234;
		printf("									^ No.%ud productor done ... data is : %d, index = %d\n", pthread_self(), buf[index], index);
		sem_post(&data_nums);
		++index;
		index %= _SIZE;
		pthread_mutex_unlock(&producting);
	}
}

int main()
{
	sem_init(&space_nums, 0, _SIZE);
	sem_init(&data_nums, 0, 0);

	pthread_mutex_init(&consuming, NULL);
	pthread_mutex_init(&producting, NULL);



// 4 consumer, 2 productor	
	pthread_t p_id1, p_id2, p_id3, p_id4;
	pthread_t c_id1, c_id2;

	pthread_create(&p_id1, NULL, &consumer, NULL);
//	pthread_create(&p_id2, NULL, &consumer, NULL);
//	pthread_create(&p_id3, NULL, &consumer, NULL);
//	pthread_create(&p_id4, NULL, &consumer, NULL);
	pthread_create(&c_id1, NULL, &productor, NULL);
	pthread_create(&c_id2, NULL, &productor, NULL);
//
	pthread_join(p_id1, NULL);
//	pthread_join(p_id2, NULL);
//	pthread_join(p_id3, NULL);
//	pthread_join(p_id4, NULL);
	pthread_join(c_id1, NULL);
//	pthread_join(c_id2, NULL);

	sem_destroy(&space_nums);
	sem_destroy(&data_nums);

	return 0;
}
