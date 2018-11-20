#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define MAXUSERS 30

typedef struct{
    sem_t sem1;
    sem_t sem2;
    sem_t aux;
}   semaforos;

int requerir(semaforos *sem){
        if(sem_trywait(&sem->sem1) == 0){
            return 1;
        }
        if(sem_trywait(&sem->sem2) == 0){
            return 2;
        }
}

void liberar(int impresora,semaforos *sem){//desbloqueo la impresora
    if(impresora==1){
        sem_post(&sem->sem1);
    }
    if(impresora==2){
        sem_post(&sem->sem2);
    }
}

void imprimir(int impresora){//imprimo
    printf("estoy imprimiento en la impresora %d.\n",impresora);
    sleep(rand()%4);
    printf("termine de imprimir en la impresora %d.\n",impresora);

}

void *hilo(void *sem){
    sem_wait(&((semaforos*)sem)->aux);//si hay alguna impresora libre
    int n =requerir((semaforos*)sem);//bloqueo la impresora
    imprimir(n);//seccion critica
    liberar(n,(semaforos*)sem);//desbloqueo la impresora
    sem_post(&((semaforos*)sem)->aux);
    pthread_exit(EXIT_SUCCESS);
}

int main()
{
    pthread_t threads[MAXUSERS];
    semaforos *sem = (semaforos *)malloc(sizeof(semaforos));
    sem_init(&sem->sem1,0,1);//semaforo de la impresora 1
    sem_init(&sem->sem2,0,1);//semaforo de la impresora 2
    sem_init(&sem->aux,0,2);//semaforo auxiliar, cuenta la cantidad de impresoras libres

    int i;
    for(i=0;i<MAXUSERS;i++){
        pthread_create(&threads[i],NULL,hilo,sem);
    }

    for (i = 0; i < MAXUSERS; i++) {
		pthread_join(threads[i], NULL);
    }
    sem_destroy(&sem->sem1);
    sem_destroy(&sem->sem2);
    sem_destroy(&sem->aux);
    free(sem);
    return 0;
}
