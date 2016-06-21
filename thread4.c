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

void increment_var2()
{
	pthread_mutex_lock(&lock);
	printf("test.var2 = %d\n", ++test.var2);
	pthread_mutex_unlock(&lock);
}

void* thread1(void* arg)
{	
	while (1) {
		pthread_mutex_lock(&lock);
		printf("thread1 lock\n");
		printf("test.var2 = %d\n", ++test.var2);
		printf("thread1 unlock\n");
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return NULL;
}

/**
 *使用递归锁，该线程在获取锁过后，去调用另外一个同步方法。
 *在该方法中，也试图去获得锁，这里一个线程多次去获得一个
 * 递归锁，不会造成死锁。
 */
void* thread2(void* arg)
{
	while (1) {
		pthread_mutex_lock(&lock);
		printf("thread2 lock\n");
		increment_var2();
		int res = test.var1 + test.var2;
		printf("res = %d\n", res);
		printf("thread2 unlock\n");
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_mutexattr_t attr;  
    pthread_mutexattr_init(&attr);  
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&lock, &attr);
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