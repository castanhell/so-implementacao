#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/* fun��o que tratar� os sinais recebidos */
void tratador (int signum)
{
   printf ("Recebi o sinal %d\n", signum) ;
}

int main (void)
{
   struct sigaction action;

   /* Configura a estrutura que especifica a nova a��o */
   action.sa_handler = tratador;
   sigemptyset (&action.sa_mask);
   action.sa_flags = 0;

   /* registra a��o para o sinal SIGINT (^C do teclado) */
   if (sigaction (SIGINT, &action, 0) < 0)
   {
      perror ("Erro em sigaction: ") ;
      exit (1) ;
   }

   /* la�o vazio */
   while (1) ;
}
