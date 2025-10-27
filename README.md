# Operating Systems Assignment 1
#### Created By: Logan Senol

A solution to the classic producer consumer problem implementing pthread and semaphores coded in C++


## Installation and Compilation
You can clone this repository by running
```bash
git clone https://github.com/Goatmanwaffles/OperatingSystemsAssignment1
```

Once installed, open a terminal window in a Linux-based environment and navigate to the cloned repository. You can then compile both .cpp files with

```bash
g++ -o producer producer.cpp -pthread
g++ -o consumer consumer.cpp -pthread
```
(Any C++ compiler should work but only tested with g++)

This will create 2 executables that you can then run in separate terminals with
```bash
./producer
./consumer
```

## Purpose of this program
This program aims to demonstrate the implementation of shared memory between two programs through a simple producer consumer relationship. Memory is reserved in the system for our producer process to fill with incrementing numbers, while our consumer then removes those numbers from memory, clearing space for the producer to keep going. This implementation has a producer buffer size of 2, meaning only 2 numbers will ever be stored in memory at once.

## Explanation of the Program
This program is comprised of three seperate files
#### SharedData.hpp
This .hpp files defines the structure of our shared data between the two files, as well as our total buffer size (the number of data to be produced at once) and the size of the reserved memory for the program. Our SharedData structure holds an array of BUFFERSIZE to hold the data and a counter variable to track how many pieces of data are currently being stored. The structure also holds 5 semaphores to manage critical section processing and ensure data stability between the producer and consumer. The full and empty semaphores track the states of producing and consuming data, if data is being made or consumed actively. The canProduce and canConsume are used to implement batch processing between producer and consumer and only unlock once all data is filled or emptied, and the mutex semaphore tracks the actively running program and ensure neither of the two can run at the same time.

#### producer.cpp
This file defines the behavior of our producer program that fills our SharedData structure, as well as acting as the initlization for the processes together, initlizing semaphores and reserving memory for the set of processes. The produceItems function initilizes a instance of our SharedData structure and tracks an item integer to track the value of the next item to produce. We then go through a series of semaphore checks for memory saftey, checking if consumer is done, the buffer is empty, and if the mutex lock is free. Then the program appends items to our buffer till it hits the max size defined, and posts the relevant semaphores. While doing this is prints out messages to the console to make this process easy to track. The main function of this file also reserves memory using shm_open, we then map that memory object to a void pointer using mmap so that out producer and consumer functions may affect the memory. Main then resets and clears the reserved memory and initilizes the semaphore vales before running the produceItems function starting the producer consumer cycle.

#### consumer.cpp
This file define the behavior of our consumer program that removes the values from SharedData and prints them out. It follows much the same logic as the producer, simpily in reverse. the consumeItems function checks if the producer is done running and if the buffer is filled. It then iterates through each item in the buffer starting at the highest index item and prints out the value before removing it from memory, essentially "consuming" the value. Once it is done going through the buffer it marks all the relevant semaphores and allows the producer to run again, continuing the loop. The main function of this program connects to the shared memory created by producer and maps the data to a pointer in the same way. No initilization for semaphore of any kind is done in this program.  

#### Usage of Semaphores
This program makes use of semaphore based locks to ensure proper memory protection through mutal exclusion. The shared semaphores between producer and consumer ensure that neither of the programs are able to modify memory at the same time as they lock each other out when one of the processes critical sections start. This back and forward locking mechanism ensures mutal exclusion between both processes.
