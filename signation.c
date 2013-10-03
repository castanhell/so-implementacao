#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* função que tratará os sinais recebidos */
void tratador (int signum)
{
   printf ("Recebi o sinal %d\n", signum) ;
}

int main (void)
{
   struct sigaction action;

   /* Configura a estrutura que especifica a nova ação */
   action.sa_handler = tratador;
   sigemptyset (&action.sa_mask);
   action.sa_flags = 0;

   /* registra ação para o sinal SIGINT (^C do teclado) */
   if (sigaction (SIGINT, &action, 0) < 0)
   {
      perror ("Erro em sigaction: ") ;
      exit (1) ;
   }

   /* laço vazio */
   while (1) ;
}
