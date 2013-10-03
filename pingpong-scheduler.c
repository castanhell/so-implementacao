#include <stdio.h>
#include <stdlib.h>
#include "task.h"

task_t Pang, Peng, Ping, Pong, Pung ;

int prioridade = 0 ;

void Body (void * arg)
{
   int i ;

   task_nice (prioridade);
   prioridade += 2 ;

   for (i=0; i<10; i++)
   {
      printf ("%s %d\n", (char *) arg, i) ;
      task_yield ();
   }
   printf ("%s FIM\n", (char *) arg) ;
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   printf ("Main INICIO\n");

   task_init () ;

   task_create (&Pang, Body, "    Pang") ;
   task_create (&Peng, Body, "        Peng") ;
   task_create (&Ping, Body, "            Ping") ;
   task_create (&Pong, Body, "                Pong") ;
   task_create (&Pung, Body, "                    Pung") ;

   task_yield () ;

   printf ("Main FIM\n");
   exit (0) ;
}
