#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define MAXUSERS 30

pthread_t threads[MAXUSERS];
typedef struct{
    sem_t sem1;
    sem_t sem2;
}   semaforos;

int requerir(void *sem){
    while(1){
        if(sem_trywait(&((semaforos *)sem)->sem1) == 0){
            return 1;
        }
        if(sem_trywait(&((semaforos *)sem)->sem2) == 0){
            return 2;
        }
    }
}

void liberar(int impresora,void *sem){
    if(impresora==1){
        sem_post(&((semaforos *)sem)->sem1);
    }
    if(impresora==2){
        sem_post(&((semaforos *)sem)->sem2);
    }
}

void imprimir(int impresora){
    printf("estoy imprimiento en la impresora %d.\n",impresora);
}

void *hilo(void *sem){
    int n =requerir(sem);
    imprimir(n);
    liberar(n,sem);
    pthread_exit(EXIT_SUCCESS);
}

int main()
{
    semaforos *sem = (semaforos *)malloc(sizeof(semaforos));
    sem_init(&sem->sem1,0,1);
    sem_init(&sem->sem2,0,1);

    int i;
    for(i=0;i<MAXUSERS;i++){
        pthread_create(&threads[i],NULL,hilo,sem);
    }

    for (i = 0; i < MAXUSERS; i++) {
		pthread_join(threads[i], NULL);
    }
    sem_destroy(&((semaforos *)sem)->sem1);
    sem_destroy(&((semaforos *)sem)->sem2);
    free(sem);
    return 0;
}
