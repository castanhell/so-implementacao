#ifndef __TASK__
#define __TASK__

#ifndef NULL
#define NULL ((void *) 0)
#endif

#include <ucontext.h>
#define STACKSIZE 65536

/*
* Define a estrutura de uma tarefa
*/

typedef struct task_t
{
   struct task_t *prev, *next ;   // para poder enfileirar tarefas mais tarde
   int tid ;         // ID da tarefa
   ucontext_t context; //Contexto da tarefa
   int priority;
   short int systemTask;
   long int runningTime;
   int activations;
} task_t ;

void task_init();

int task_create (task_t * task, void (*start_routine)(void *),  void * arg);

int task_switch (task_t *task);

void task_exit (int exit_code);

void task_yield ();

int task_id ();

int task_nice(int nice_level);

void dispatcher_body();

task_t *scheduler();

void startTimer();

void endTimer();

#endif
