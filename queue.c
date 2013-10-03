#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

void queue_append(queue_t **queue, queue_t *elem){
   /* O elemento deve existir */
   
   if(elem->next!=NULL || elem->prev !=NULL){
       return;
   }
   //assert( elem != NULL);
   /* O elemento nao deve estar em outra fila ?? */
   /* faz o append */
   //Fila vazia
   if(!(*queue)){
       (*queue) = elem;
       //adiciona
       (*queue)->next = elem;
       (*queue)->prev = elem;
       return;
   }
   //Fila nao vazia
   else{
       //Fila unica
       queue_t *prevAnterior = (*queue)->prev;
       (*queue)->prev = elem;
       elem->prev = prevAnterior;
       elem->next = (*queue);
       //troca o next do prevAnterior
       prevAnterior->next = elem;
       return;
   }
}

queue_t *queue_remove(queue_t **queue, queue_t *elem){
    //Fila vazia
    if((*queue)==NULL){
        return NULL;
    }
    //Fila Unica
    if(((*queue)->prev==(*queue)->next) && ((*queue)==(*queue)->next) && (*queue)==(elem)){
        queue_t *return_value = (*queue)->next;
        (*queue) = NULL;
        return_value->prev = NULL;
        return_value->next = NULL;
        return return_value;
    }
    //Fila com 2+ elementos
    else{
        queue_t *removed = NULL;
        queue_t *queueElement = *(queue);
        do{
            if(queueElement==elem){
                //Arruma a fila
                queueElement->prev->next = queueElement->next;
                queueElement->next->prev = queueElement->prev;
                removed = queueElement;
                //Se for a origem da fila
                if(removed==(*queue)){
                    (*queue) = removed->next;
                }
                removed->next = NULL;
                removed->prev = NULL;
                break;
            }
            queueElement=queueElement->next;
        }while(queueElement!=(*queue));
        return removed;
    }
}

int queue_size(queue_t *queue){
    if(queue==0){
        return 0;
    }
    else{
        int size = 0;
        queue_t *queueTmp = queue;
        do{
            size++;
            queueTmp = queueTmp->next;
        } while(queueTmp!=queue);
        return size;
    }
}

void queue_print(char *name, queue_t *queue, void print_elem (void*)){
    printf("%s:[",name);
    queue_t *elem = queue;
    if(queue != NULL){
    	do{
            print_elem(elem);
            elem = elem->next;
            printf(" ");
        } while(elem!=queue);
    }
    printf("]\n");
    return;
}
