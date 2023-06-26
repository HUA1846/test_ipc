#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
// #include "shared.h"

#define SHM_SIZE 1024  // Size of the shared memory segment
#define SEM_NAME "/my_semaphore"

int main() {
    int shm_id;
    sem_t* semaphore;
    
    // Open the shared memory segment
    shm_id = shm_open("/my_shared_memory", O_RDWR, 0666);
    if (shm_id == -1) {
        perror("shm_open");
        exit(1);
    }
    
    // Map the shared memory segment into the process's address space
    char* shared_memory = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    // Open the semaphore
    semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    
    // Acquire the semaphore before reading
    sem_wait(semaphore);
    
    // Read the message from the shared memory
    printf("Reader: Message received: %s\n", shared_memory);
    
    // Release the semaphore after reading
    sem_post(semaphore);
    
    // Clean up resources
    sem_close(semaphore);
}