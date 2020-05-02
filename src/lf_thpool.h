#ifndef _LF_THPOOL_H
#define _LF_THPOOL_H

#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


typedef struct {
    void (*function)(void *);
    void *arg;
} task_t;

/* define a thread structure */
typedef struct {
    task_t *buffer;           // workqueue
    pthread_t thr;            // the actual thread
    int id;                   // thread id
    int size;                 // size of queue
    unsigned int in;          // index of pushing a task
    unsigned int out;         // index of popping a task
    volatile int task_count;  // number of tasks in queue
} thread_t;

/* define a thread pool structure */
typedef struct {
    thread_t *threads;
    int thread_count;
    int is_stopped;
} thpool_t;

/* utils */
thpool_t *thpool_create(int thread_count, int queue_size);
int thpool_destroy(thpool_t *lf_thpool);
task_t *thpool_deq(thread_t *thread);
void thpool_enq(thpool_t *lf_thpool, void (*task)(void *), void *arg);
thread_t *round_robin_schedule(thpool_t *lf_thpool);
int dispatch_task(thread_t *thread, void (*task)(void *), void *arg);

#endif