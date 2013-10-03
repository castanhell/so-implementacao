#include <stdio.h>
#include <stdlib.h>
#include "task.h"

task_t Pang, Peng, Ping, Pong, Pung ;

void Body (void * arg)
{
   int i,j ;

   for (i=0; i<10; i++)
   {
      printf ("%s %d\n", (char *) arg, i) ;
      for (j=0; j<10000000; j++) ;
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
   exit (0);
}
