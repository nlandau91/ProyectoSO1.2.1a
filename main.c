#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define MAXUSERS 30

pthread_t threads[MAXUSERS];
sem_t sem1;
sem_t sem2;

int requerir(){
    while(1){
        if(sem_trywait(&sem1) == 0){
            return 1;
        }
        if(sem_trywait(&sem2) == 0){
            return 2;
        }
    }
}

void liberar(int impresora){
    if(impresora==1){
        sem_post(&sem1);
    }
    if(impresora==2){
        sem_post(&sem2);
    }
}

void imprimir(int impresora){
    printf("estoy imprimiento en la impresora %d.\n",impresora);
}

void *hilo(){
    int n =requerir();
    imprimir(n);
    liberar(n);
    pthread_exit(EXIT_SUCCESS);
}

int main()
{
    sem_init(&sem1,0,1);
    sem_init(&sem2,0,1);

    int i;
    for(i=0;i<MAXUSERS;i++){
        pthread_create(&threads[i],NULL,hilo,NULL);
    }

    for (i = 0; i < MAXUSERS; i++) {
		pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
