#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "sharedData.hpp"
#include <cstring>

//Function to produce items into memory
void produceItems(void* arg){
     SharedData* sm = static_cast<SharedData*>(arg);
     int item = 0;
     while(true){
	  sem_wait(&sm->canProduce);

	  while (sm->count < BUFFER_SIZE) {
               sem_wait(&sm->empty);
	       sem_wait(&sm->mutex);

	       sm->buffer[sm->count] = ++item;
	       sm->count++;
	       std::cout << "Produced: " << item << " Count: " << sm->count << std::endl;

	       sem_post(&sm->mutex);
	       sem_post(&sm->full);
          }
     std::cout << "Done producing handing to consumer" << std::endl;
     sem_post(&sm->canConsume);
     }
     
}

int main(){
     //Create shared memory
     int shm_fc;
     shm_fc = shm_open("/MEMPC", O_CREAT | O_RDWR, 0666); //Creates shared memory 
	if (shm_fc == -1) {
		std::cout << "FAILED TO RUN SHM_OPEN\n";
	}

     ftruncate(shm_fc, sizeof(SharedData)); //Reduces size of shared memory to 1000 bytes
	if(ftruncate(shm_fc, sizeof(SharedData)) == -1){
		std::cout << "FAILED TO TRIM DATA\n";
	}

     void* ptr = mmap(nullptr, sizeof(SharedData), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fc, 0); //Pointer to shared memory object
	if (ptr == MAP_FAILED) {
	std::cout << "COULD NOT PROPERLY MAP POINTER\n";
	}
     
     auto sm = static_cast<SharedData*>(ptr);
	
     //Memory Protection
     memset(sm, 0, sizeof(SharedData));
     //Create semaphore locks
     sem_init(&sm->mutex,1 , 1);
     sem_init(&sm->full, 1, 0);
     sem_init(&sm->empty, 1, BUFFER_SIZE);
     sem_init(&sm->canProduce,1,1);
     sem_init(&sm->canConsume,1, 0);
     //Produce items
     produceItems(sm);

     munmap(ptr, sizeof(SharedData));
     close(shm_fc);
     return 0;
}
