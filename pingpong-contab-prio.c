#include <stdio.h>
#include <stdlib.h>
#include "task.h"

task_t Pang, Peng, Ping, Pong, Pung ;

int prioridade = 0 ;

void Body (void * arg)
{
   int i,j ;

   task_nice (prioridade);
   prioridade -= 2 ;

   printf ("%s INICIO em %4d ms\n", (char *) arg, systime()) ;

   for (i=0; i<40000; i++)
      for (j=0; j<40000; j++) ;

   printf ("%s FIM    em %4d ms\n", (char *) arg, systime()) ;
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
   exit (0);
}
