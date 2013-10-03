#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

// tratador do sinal
void tratador (int signum)
{
   printf ("Recebi o sinal %d\n", signum) ;
}

int main ()
{
   struct itimerval timer;
   struct sigaction action ;

   // define a ação para o sinal de timer
   action.sa_handler = tratador;
   sigemptyset (&action.sa_mask);
   action.sa_flags = 0;
   if (sigaction (SIGALRM, &action, 0) < 0)
   {
      perror ("Erro em sigaction: ") ;
      exit (1) ;
   }

   // ajusta valores do temporizador
   timer.it_value.tv_usec = 0 ;      // primeiro disparo, em micro-segundos
   timer.it_value.tv_sec  = 3 ;      // primeiro disparo, em segundos
   timer.it_interval.tv_usec = 1000000 ;   // disparos subsequentes, em micro-segundos
   timer.it_interval.tv_sec  = 0 ;   // disparos subsequentes, em segundos

   // arma o temporizador ITIMER_REAL (vide man setitimer)
   if (setitimer (ITIMER_REAL, &timer, 0) < 0)
   {
      perror ("Erro em setitimer: ") ;
      exit (1) ;
   }

   // laco vazio
   while (1) ;
}
