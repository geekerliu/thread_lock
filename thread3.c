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
pthread_t thread_id3;
pthread_mutex_t lock;

void increment_var1()
{
	pthread_mutex_lock(&lock);
	printf("test.var1 = %d\n", ++test.var1);
	pthread_mutex_unlock(&lock);
}

void increment_var2()
{
	pthread_mutex_lock(&lock);
	printf("test.var2 = %d\n", ++test.var2);
	pthread_mutex_unlock(&lock);
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

/**
 *使用非递归锁，该线程在获取锁过后，去调用另外一个同步方法。
 *在该方法中，也试图去获得锁，这里一个线程多次去获得一个
 * 非递归锁，造成死锁。
 */
void* thread3(void* arg)
{
	while (1) {
		pthread_mutex_lock(&lock);
		test.var1 = 1;
		increment_var2();
		int res = test.var1 + test.var2;
		printf("res = %d\n", res);
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_mutex_init(&lock, NULL);
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
	ret = pthread_create(&thread_id3, NULL, thread3, NULL);
	if (ret < 0) {
		fprintf(stderr, "pthread_create failed\n");
		return -1;
	}
	while (1) {
		sleep(50);
	}

	return 0;
}