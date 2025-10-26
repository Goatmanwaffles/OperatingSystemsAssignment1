#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "sharedData.hpp"

void consumeItems(void* arg){
     SharedData* sm = static_cast<SharedData*>(arg);
     int item;
     while(true){
	  sem_wait(&sm->canConsume);
	  while(sm->count > 0){
               sem_wait(&sm->full);
	       sem_wait(&sm->mutex);

	       sm->count -= 1;
               item = sm->buffer[sm->count];

	       std::cout << "Consumed: " << item << " Count: " << sm->count << std::endl;

	       sem_post(&sm->mutex);
	       sem_post(&sm->empty);
	  }
	  sleep(2);
     
     std::cout << "Buffer Consumed. Handing to Producer" << std::endl;
     sem_post(&sm->canProduce);
     }
}

int main(){
     int shm;
     shm = shm_open("MEMPC", O_CREAT | O_RDWR, 0666); //Creates shared memory object
     void* ptr = mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED,shm, 0);

     auto sm = static_cast<SharedData*>(ptr);

     consumeItems(sm);
     munmap(ptr, sizeof(SharedData));
     close(shm);
     return 0;
}
