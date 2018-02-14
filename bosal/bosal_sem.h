#ifndef BOSAL_SEM_H
#define BOSAL_SEM_H 1

#include "bosal.h"

typedef struct tag_sem_t *sem_t;

int sem_init(sem_t *psem, int flags, unsigned count);
int sem_destroy(sem_t *psem);
int sem_post(sem_t *psem);
int sem_trywait(sem_t *psem);
int sem_wait(sem_t *psem);
int sem_timedwait(sem_t *psem, const struct timespec *abstim);

#endif
