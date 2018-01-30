#include "bnetheaders.h"
#include "bosal.h"

static bos_interface_t *tface;
static bos_thread_t thread1, thread2;
static bos_msgq_t msgq;
static bos_sem_t xsem;

int t1func(void *arg)
{
	bos_msgq_t msgq;
	bos_msg_t  msg;
	int counter = 0;
	int result;

	msgq = (bos_msgq_t)arg;

	while (1)
	{
		tface->sleep(5, 0);
		printf("T1\n");
		msg.msg = counter++;
		result = tface->msgq_send(msgq, &msg);
		if (result < 0)
		{
			fprintf(stderr, "T1 msgq tx fail\n");
			return -1;
		}
	}
	return 0;
}

int t2func(void *arg)
{
	bos_msgq_t msgq;
	bos_msg_t  msg;
	int result;

	msgq = (bos_msgq_t)arg;

	while (1)
	{
		result = tface->msgq_receive(msgq, &msg, 2, 0);

		if (result < 0)
		{
			fprintf(stderr, "T2 msgq rx fail\n");
			return -1;
		}
		else if (result > 0)
		{
			printf("T2\n");
		}
		else
		{
			printf("Msg from T1=%u\n", msg.msg);
			if (msg.msg == 5)
			{
				result = tface->semaphore_put(xsem);

				if (result < 0)
				{
					fprintf(stderr, "T2 sem put fail\n");
					return -1;
				}
			}
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	int result;

	tface = bosal_interface();
	if (! tface)
	{
		fprintf(stderr, "No interface\n");
		return -1;
	}
	// create semaphore
	//
	xsem = tface->semaphore_create(0);
	if (! xsem)
	{
		fprintf(stderr, "No sem\n");
		return -1;
	}
	// create msgq
	//
	msgq = tface->msgq_create(10);
	if (! msgq)
	{
		fprintf(stderr, "No msgq\n");
		return -1;
	}
	// create thread 1
	//
	thread1 = tface->thread_create(t1func, 4095, BOSAL_MIDDLE_PRIORITY, msgq, "thread1");
	if (! thread1)
	{
		fprintf(stderr, "No thread1\n");
		return -1;
	}
	// create thread 2
	//
	thread2 = tface->thread_create(t2func, 4095, BOSAL_MIDDLE_PRIORITY, msgq, "thread2");
	if (! thread1)
	{
		fprintf(stderr, "No thread1\n");
		return -1;
	}
	// wait for signal to exit
	//
	while (1)
	{
		result = tface->semaphore_get(xsem, 10, 0);

		if (result < 0)
		{
			fprintf(stderr, "sem get fail\n");
			return -1;
		}
		if (result == 0)
		{
			printf("test over\n");
			break;
		}
		printf("Sleepy..\n");
	}
	return 0;
}

