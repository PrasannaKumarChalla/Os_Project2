#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define NO_OF_PHILOSOPHERS 4
void * threadFunc(int n);        //function for thread
pthread_t  philosophers[NO_OF_PHILOSOPHERS];      //4 threads for 4 philosophers
sem_t lock;                      //semaphore lock
pthread_mutex_t chopsticks[NO_OF_PHILOSOPHERS];         //4 mutexes for 4 chopsticks

int main(int argc, char const *argv[]){

    int wtime,i,res;
    void * msg;

 // if (argc < 2){

 //      printf("You must specify a number of times to run the loop");
 //      exit(1); // stop if they didn't give us a number
 //    }
 // wtime = atoi(argv[1]);
	
sem_init(&lock,0,NO_OF_PHILOSOPHERS-1);
 for(i=0;i<NO_OF_PHILOSOPHERS;i++){
     res=pthread_mutex_init(&chopsticks[i],NULL);
     if(res==-1){
     	printf("\n Mutex initialization failed");
        exit(1);
     }
 }
 for(i=0;i<NO_OF_PHILOSOPHERS;i++){
 	res=pthread_create(&philosophers[i],NULL,(void *)threadFunc,(int *)i);
     if(res!=0){
     	printf("\n Thread creation failed:");
        exit(1);
     }
 }
 for (i = 0; i < NO_OF_PHILOSOPHERS; i++){
 	res=pthread_join(philosophers[i],&msg);
 	if(res!=0){
        printf("\n Thread join Failed");
        exit(1);
 	}
 }
 for(i=0;i<NO_OF_PHILOSOPHERS;i++){
    res=pthread_mutex_destroy(&chopsticks[i]);
    if(res!=0){
    	printf("\n Mutex destroy problem \n");

    }
 }
    sem_destroy(&lock);
	return 0;
}

void * threadFunc(int n){
	int count=0;
	printf("\nPhilosopher %d is joining Table",n);
	while(count<NO_OF_PHILOSOPHERS){
	   sem_wait(&lock);
	   pthread_mutex_lock(&chopsticks[n]);
	   pthread_mutex_lock(&chopsticks[(n+1)%NO_OF_PHILOSOPHERS]);
	   printf("\nPhilosopher %d is Eating",n);
	   usleep(3);
	   pthread_mutex_unlock(&chopsticks[n]);
	   pthread_mutex_unlock(&chopsticks[(n+1)%NO_OF_PHILOSOPHERS]);
	   sem_post(&lock);
	   printf("\nPhilosopher %d is pondering the meaning of life; count %d",n,count);
	   usleep(3);
	   count++;
    }
    printf("\nPhilosopher %d is leaving Table",n);
}