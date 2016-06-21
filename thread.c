#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct _test {
	int var1;
	int var2;
	int another;
} test = {
	.var1 = 0,
	.var2 = 0
};

pthread_t thread_id1;
pthread_t thread_id2;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

void increment_var1()
{
	pthread_mutex_lock(&lock1);
	test.var1++;
	printf("test.var1 = %d\n", test.var1);
	pthread_mutex_unlock(&lock1);
}

void increment_var2()
{
	pthread_mutex_lock(&lock2);
	test.var2++;
	printf("test.var2 = %d\n", test.var2);
	pthread_mutex_unlock(&lock2);
}

void* thread1(void* arg)
{	
	while (1) {
		increment_var1();
		sleep(1);
	}
	return NULL;
}

void* thread2(void* arg)
{
	while (1) {
		increment_var2();
		sleep(1);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);
	int ret = pthread_create(&thread_id1, NULL, thread1, NULL);
	if (ret < 0) {
		fprintf(stderr, "pthread_create failed\n");
		return -1;
	}
	ret = pthread_create(&thread_id2, NULL, thread2, NULL);
	if (ret < 0) {
		fprintf(stderr, "pthread_create failed\n");
		return -1;
	}
	while (1) {
		sleep(50);
	}

	return 0;
}