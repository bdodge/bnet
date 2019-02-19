/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
