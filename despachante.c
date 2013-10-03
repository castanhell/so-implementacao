#include <stdio.h>
#include <stdlib.h>
#include "task.h"

task_t Ping, Pong ;

void BodyPing (void * arg)
{
   int i ;
   char* name = (char *) arg ;

   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", name, i) ;
      task_switch (&Pong);
   }
   printf ("%s FIM\n", name) ;
   task_exit (0) ;
}

void BodyPong (void * arg)
{
   int i ;
   char* name = (char *) arg ;

   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", name, i) ;
      task_switch (&Ping);
   }
   printf ("%s FIM\n", name) ;
   task_exit (0) ;
}

int main (int argc, char *argv[])
{
   printf ("Main INICIO\n");

   task_init () ;

   task_create (&Ping, BodyPing, "    Ping") ;
   task_create (&Pong, BodyPong, "        Pong") ;

   task_switch (&Ping) ;
   task_switch (&Pong) ;

   printf ("Main FIM\n");

   exit (0);
}
