#include <stdio.h>
#include <string.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <printf.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SHM_SIZE 1024  // Size of the shared memory segment
#define SEM_NAME "/my_semaphore"

int main() {
    int shm_id;
    sem_t* semaphore;
    
    // Create or open the shared memory segment
    shm_id = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    if (shm_id == -1) {
        perror("shm_open");
        exit(1);
    }
    
    // Set the size of the shared memory segment
    if (ftruncate(shm_id, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }
    
    // Map the shared memory segment into the process's address space
    char* shared_memory = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    // Open or create the semaphore
    semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    
    // Acquire the semaphore before writing
    sem_wait(semaphore);
    
    // Write a message to the shared memory
    strcpy(shared_memory, "Hello, shared memory!");
    printf("Writer: Message written to shared memory.\n");
    
    // Release the semaphore after writing
    sem_post(semaphore);
    
    // Clean up resources
    // sem_close(semaphore);
    // munmap(shared_memory, SHM_SIZE);
    // shm_unlink("/my_shared_memory");
    
    return 0;
}