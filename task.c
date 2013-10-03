#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include "task.h"
#include "queue.h"
#include "time.h"

int id = 0;

task_t Main, dispatcher;

task_t *currentTask;

task_t *rdyQueue = NULL;
task_t *currentQueue = NULL;

int quantum, taskStart, taskFinish;

/*
 * Trata o timer atual
 * */

void handleTimer(int signum){
	//Incrementa o timer
	tick();
	if(currentTask->systemTask){
		//Ignora
	}
	else{
		if(quantum == 0){
			#ifdef DEBUG
			printf("Liberando tarefa %d\n", currentTask->tid);
			#endif
			//Termina
			endTimer();
			task_yield();
		}
		else{
			quantum--;
		}
	}
}

void dispatcher_body(){
	#ifdef DEBUG
	printf("Dispatcher Inicio\n");
	printf("Queue Size: %d\n",queue_size((queue_t*)rdyQueue));
	#endif

	while(queue_size((queue_t*)rdyQueue) > 0){
		task_t *next = scheduler();
		if(next){
			//Inicia o quantum
			quantum = 20;
			//Abre o timer para a próxima tarefa
			//Obs: Aproximado!
			startTimer();
			task_switch(next);
		}
	}
	task_exit(0);
}

void changePriority(task_t *task, int factor){
	if(task->priority < 20 && task->priority > -20){
		task->priority += factor;
	}
	else{
		//Corrige para valor valido
		task->priority = 0;
	}
}

task_t *scheduler(){
	//se existe uma fila não
	//Vazia
	if(rdyQueue){
		task_t *nextElement = rdyQueue;
		task_t *menorElement = NULL;
		int maiorP = -21;
		do{
			if(nextElement->priority > maiorP){
				maiorP = nextElement->priority;
				menorElement = nextElement; 
			}
			nextElement = nextElement->next;
		} while(nextElement != rdyQueue);
		//Envelhece as demais
		nextElement = rdyQueue;
		do{
			if(nextElement->tid != menorElement->tid){
				if(nextElement->priority < 20 && nextElement->priority > -20)
					changePriority(nextElement,1);
				else{  //Reseta para uma prioridade válida
					nextElement->priority = 0;	
				}
			}
			nextElement = nextElement->next;
		} while(nextElement != rdyQueue);
		//Troca o topo da lista (rdyQueue)
		rdyQueue = menorElement;
		//Retorna

		return menorElement;
	}
	return 0;
}

void startTimer(){
	//Inicia o timer
    taskStart = systime();
}

void endTimer(){
	//Finaliza o timer da tarefa
	taskFinish = systime();
	currentTask->runningTime += taskFinish-taskStart;
	currentTask->activations++;
	taskStart = 0;
	taskFinish = 0;
}

void task_init(){
    /* desativa o buffer da saida padrao (stdout), usado pela função printf */
	setvbuf (stdout, 0, _IONBF, 0) ;
	//Cria a tarefa main!
	task_t main;
	main.prev = NULL;
	main.next = NULL;
    main.tid = id;
    
	ucontext_t context;

	getcontext(&context);
    
	main.context = context;
	
	Main = main;
	
	currentTask = &Main;

	main.systemTask = 0;
	
	#ifdef DEBUG
	printf("Criou tarefa Main(%d)\n",main.tid);
	#endif

	//Cria a tarefa dispatcher
	dispatcher.prev = NULL;
	dispatcher.next = NULL;
	dispatcher.tid = ++id;

	ucontext_t dispatchercont;

	getcontext(&dispatchercont);

    char *stack;
	stack = malloc(STACKSIZE);
	if(stack){
		dispatchercont.uc_stack.ss_sp = stack;
		dispatchercont.uc_stack.ss_size = STACKSIZE;
		dispatchercont.uc_stack.ss_flags = 0;
		dispatchercont.uc_link = 0;
	}
	else{
		perror("Erro na criação da pilha");
	}

	dispatcher.systemTask = 1;

	//Inicia contexto
	makecontext(&dispatchercont, (void*) (dispatcher_body), id);
	dispatcher.context = dispatchercont;
	//Inicia o timer
	struct itimerval timer;
	struct sigaction action;
	//Define a ação para o sinal de timer
	action.sa_handler = handleTimer; 
	sigemptyset (&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction (SIGALRM, &action, 0) < 0)
	{
	perror ("Erro em sigaction: ") ;
	exit (1) ;
	}

	// ajusta valores do temporizador
	timer.it_value.tv_usec = 100 ;      // primeiro disparo, em micro-segundos
	timer.it_value.tv_sec  = 0 ;      // primeiro disparo, em segundos
	timer.it_interval.tv_usec = 1000;   // disparos subsequentes, em micro-segundos
	timer.it_interval.tv_sec  = 0 ;   // disparos subsequentes, em segundos

	// arma o temporizador ITIMER_REAL (vide man setitimer)
	if (setitimer (ITIMER_REAL, &timer, 0) < 0)
	{
	perror ("Erro em setitimer: ") ;
	exit (1) ;
	}
}

int task_create (task_t * task, void (*start_routine)(void *),  void * arg){
	//Cria a tarefa (Não para a main)
	if(task){
	   task->prev = NULL;
	   task->next = NULL;
	   id++;
	   task->tid = id;
       //Ajusta a pilha do contexto
	   ucontext_t context;
       
	   //Faz o contexto
	   getcontext(&context);

       char *stack; 
	   stack = malloc(STACKSIZE);
	   if(stack){
           context.uc_stack.ss_sp = stack;
		   context.uc_stack.ss_size = STACKSIZE;
		   context.uc_stack.ss_flags = 0;
		   context.uc_link = 0;
	   }
	   else{
           perror("Erro na criação da pilha");
		   return(-1);
	   }
       //Inicia contexto
	   makecontext(&context, (void*) (*start_routine), id, arg);
	   task->context = context;
	   #ifdef DEBUG
	   printf("Criou tarefa (%d)\n",task->tid);
	   #endif

	   //Adiciona na rdy_queue
	   
	   queue_append((queue_t **) &rdyQueue,(queue_t*) task);

	   //Seta prioridade

	   task->priority = 0;

	   task->systemTask = 0;
		task->runningTime = 0;

		task->activations = 0;

		#ifdef DEBUG
	    printf("	Tarefa adicionada na fila\n");
		#endif

	  return 1;
	}
    else{
        //Error!
	}
}

int task_switch(task_t *t){
   task_t *tmp = currentTask;
   currentTask = t;
   swapcontext(&tmp->context,&t->context);
   return 0;
}

void task_exit(int exit_code){
	//Troca para dispatcher
	//Imprime dados da tarefa
	printf("Task %d exit: execution time %d ms, processor time %d ms, %d activations\n", currentTask->tid, systime(), currentTask->runningTime, currentTask->activations);

	if(currentTask == &dispatcher){
		task_switch(&Main);
	}
	else{
		queue_remove((queue_t **) &rdyQueue,(queue_t*)currentTask);
		task_switch(&dispatcher);
	}
}

void task_yield(){
	//Volta para dispatcher
	task_switch(&dispatcher);
}

int task_id(){
   return currentTask->tid;
}

int task_nice(int nice_level){
	int oldpriority = currentTask->priority;
	if(nice_level >= -20 && nice_level <= 20){
		currentTask->priority = nice_level;
	}
	return oldpriority;
}
