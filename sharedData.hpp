#ifndef SHARED_HPP_
#define SHARED_HPP _

#include <semaphore.h>
const int BUFFER_SIZE = 2;
const int MEM_SIZE = 1000;
//Shared Data Decleration
struct SharedData {
     int buffer[BUFFER_SIZE];
     int count = 0;
     sem_t mutex;
     sem_t full;
     sem_t empty;
     sem_t canProduce;
     sem_t canConsume;
};

#endif
