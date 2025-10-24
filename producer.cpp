#include <pthread.h>
#include <semaphore.h>
//#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

#include "sharedData.hpp"

//Function to produce items into memory
void produceItems(void* arg){
     SharedData* sm = static_cast<SharedData*>(arg);
     int item;
     while(true){
          item++;
          sem_wait(&sm->empty); //Check if there is an empty spot
          sem_wait(&sm->mutex); //Ensures we have lock and only produces CR can run
          //CODE AFTER sem_wait will run once produce has verified above 2 statements

          sm->buffer[sm->count] = item; //Place item into buffer
          sm->count += 1; //increment to next buffer space

          std::cout << "Produced: " << item;
          std::cout << " Count: " << sm->count;

          //CRITICAL SECTION DONE ITEMS ADDED
          sem_post(&sm->mutex); //Unlocked
          sem_post(&sm->full); //Items added to shared memory
     }
}

int main(){
     //Create shared memory
     int shm_fc;
     shm_fc = shm_open("MEMPC", O_CREAT | O_RDWR, 0666); //Creates shared memory object
     ftruncate(shm_fc, MEM_SIZE); //Reduces size of shared memory to 1000 bytes

     void* ptr = mmap(nullptr, sizeof(SharedData), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fc, 0); //Pointer to shared memory object

     
     auto sm = static_cast<SharedData*>(ptr);

     //Create semaphore locks
     sem_init(&sm->mutex,1 , 1);
     sem_init(&sm->full, 1, 0);
     sem_init(&sm->empty, 1, BUFFER_SIZE);

     //Produce items
     produceItems(sm);
     return 0;
}