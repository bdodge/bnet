#include "bnetheaders.h"
#include "bosal.h"

#define BOSAL_MSGQ_TAG 0x1010AAAA

typedef struct tag_msgq
{
	uint32_t   	tag;
	bos_sem_t  	sem;
	bos_mtx_t	lock;
	bos_msg_t  *pool;
	size_t    	size;
	size_t    	head;
	size_t 		tail;
	size_t 	    count;
}
bmsgq_t;

bos_mtx_t bosal_mutex_create(void)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->mutex_create)
	{
		return NULL;
	}
	return b->mutex_create();
}

int bosal_mutex_destroy(bos_mtx_t mtx)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->mutex_destroy)
	{
		return -1;
	}
	return b->mutex_destroy(mtx);
}

int bosal_mutex_aquire(bos_mtx_t mtx, int tosecs, int tousecs)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->mutex_aquire)
	{
		return -1;
	}
	return b->mutex_aquire(mtx, tosecs, tousecs);
}

int bosal_mutex_release(bos_mtx_t mtx)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->mutex_release)
	{
		return -1;
	}
	return b->mutex_release(mtx);
}

bos_sem_t bosal_semaphore_create(size_t count)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->semaphore_create)
	{
		return NULL;
	}
	return b->semaphore_create(count);
}

int bosal_semaphore_destroy(bos_sem_t sem)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->semaphore_destroy)
	{
		return -1;
	}
	return b->semaphore_destroy(sem);
}

int bosal_semaphore_get(bos_sem_t sem, int tosecs, int tousecs)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->semaphore_get)
	{
		return -1;
	}
	return b->semaphore_get(sem, tosecs, tousecs);
}

int bosal_semaphore_put(bos_sem_t sem)
{
	bos_interface_t *b;

	b = bosal_interface();
	if (! b || ! b->semaphore_put)
	{
		return -1;
	}
	return b->semaphore_put(sem);
}

bos_msgq_t bosal_msgq_create(size_t max_count)
{
	bmsgq_t *q;

	if (max_count == 0)
	{
		return NULL;
	}
	q = (bmsgq_t *)malloc(sizeof(bmsgq_t));
	if (! q)
	{
		return NULL;
	}
	q->pool = (bos_msg_t *)malloc(max_count * sizeof(bos_msg_t));
	if (! q->pool)
	{
		free(q);
		return NULL;
	}
	q->lock = bosal_mutex_create();
	if (q->lock == BOSAL_INVALIDMTX)
	{
		free(q->pool);
		free(q);
		return NULL;
	}
	q->sem = bosal_semaphore_create(0);
	if (q->sem == BOSAL_INVALIDSEM)
	{
		bosal_mutex_destroy(q->lock);
		free(q->pool);
		free(q);
		return NULL;
	}
	q->tag = BOSAL_MSGQ_TAG;

	q->size  = max_count;
	q->head  = 0;
	q->tail  = 0;
	q->count = 0;
	return (void*)q;
}

int bosal_msgq_destroy(bos_msgq_t msgq)
{
	bmsgq_t *q;

	if (! msgq)
	{
		return -1;
	}
	q = (bmsgq_t *)msgq;

	if (q->tag != BOSAL_MSGQ_TAG)
	{
		return -1;
	}
	if (q->pool)
	{
		free(q->pool);
	}
	if (q->sem != BOSAL_INVALIDSEM)
	{
		bosal_semaphore_destroy(q->sem);
	}
	if (q->lock != BOSAL_INVALIDSEM)
	{
		bosal_mutex_destroy(q->lock);
	}
	free(q);
	return 0;
}

int bosal_msgq_send(bos_msgq_t msgq, bos_msg_t *msg)
{
	bmsgq_t *q;
	bos_msg_t *m;
	int result;

	if (! msgq || ! msg)
	{
		return -1;
	}
	q = (bmsgq_t *)msgq;

	if (q->tag != BOSAL_MSGQ_TAG)
	{
		return -1;
	}
	if (q->count >= q->size)
	{
		return -2;
	}
	result = bosal_mutex_aquire(q->lock, -1, -1);
	if (result)
	{
		return result;
	}
	m = &q->pool[q->head];
	m->msg 			= msg->msg;
	m->flags 		= msg->flags;
	m->param 		= msg->param;
	m->value.ullval = msg->value.ullval;
	q->head++;
	if (q->head >= q->size)
	{
		q->head = 0;
	}
	q->count++;
	result = bosal_mutex_release(q->lock);
	if (result)
	{
		return result;
	}
	result = bosal_semaphore_put(q->sem);
	return result;
}

int bosal_msgq_receive(bos_msgq_t msgq, bos_msg_t *msg, int tosecs, int tousecs)
{
	bmsgq_t *q;
	bos_msg_t *m;
	int result;

	if (! msgq || ! msg)
	{
		return -1;
	}
	q = (bmsgq_t *)msgq;

	if (q->tag != BOSAL_MSGQ_TAG)
	{
		return -1;
	}
	if (tosecs == 0 && tousecs == 0 && q->count == 0)
	{
		return 1;
	}
	result = bosal_semaphore_get(q->sem, tosecs, tousecs);
	if (result)
	{
		return result;
	}
	m = &q->pool[q->tail];
	msg->msg 		= m->msg;
	msg->flags 		= m->flags;
	msg->param 		= m->param;
	msg->value.ullval = m->value.ullval;
	q->tail++;
	if (q->tail >= q->size)
	{
		q->tail = 0;
	}
	q->count--;
	result = bosal_mutex_release(q->lock);
	return result;
}

size_t bosal_msgq_count(bos_msgq_t msgq)
{
	bmsgq_t *q;
	bos_msg_t *m;
	int result;

	if (! msgq)
	{
		return -1;
	}
	q = (bmsgq_t *)msgq;

	if (q->tag != BOSAL_MSGQ_TAG)
	{
		return -1;
	}
	return q->count;
}

