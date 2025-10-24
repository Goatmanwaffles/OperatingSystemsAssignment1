#include <pthread.h>
#include <semaphore.h>
//#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

#include "sharedData.hpp"

void consumeItems(void* arg){
     SharedData* sm = static_cast<SharedData*>(arg);
     int item;
     while(true){
          sem_wait(&sm->full); //Check if we have items to consume
          sem_wait(&sm->mutex); //Ensure we have lock and only consume CR can run
          //CODE AFTER sem_wait will run once consume has verified above 2 statements

          
          sm->count -= 1;
          item = sm->buffer[sm->count];

          std::cout << "Consumed: " << item;
          std::cout << " Count: " << sm->count <<"\n";

          //CR DONE REMOVE LOCKS
          sem_post(&sm->mutex);
          sem_post(&sm->empty);
     }
}

int main(){
     int shm;
     shm = shm_open("MEMPC", O_CREAT | O_RDWR, 0666); //Creates shared memory object
     void* ptr = mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED,shm, 0);

     auto sm = static_cast<SharedData*>(ptr);

     consumeItems(shm);
     munmap(ptr, sizeof(SharedData));
     close(shm);
     return 0;
}