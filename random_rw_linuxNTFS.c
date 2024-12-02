#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define FILE_SIZE 1024 * 1024 * 10  // 10MB file size for testing
#define BUFFER_SIZE 4096            // 4KB buffer size for each read/write operation
#define NUM_OPERATIONS 1000         // Number of random read/write operations

void random_read_write(const char* filename) {
    struct timeval start, end;

    // Open file in read-write mode
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        perror("File opening failed");
        return;
    }

    // Allocate buffer
    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Memory allocation failed");
        close(fd);
        return;
    }
    memset(buffer, 'R', BUFFER_SIZE);  // Fill buffer with dummy data

    // Start timing
    gettimeofday(&start, NULL);

    // Perform random read and write operations
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        off_t offset = (rand() % (FILE_SIZE - BUFFER_SIZE)) & ~(BUFFER_SIZE - 1);  // Align to 4KB

        // Random write
        if (lseek(fd, offset, SEEK_SET) < 0) {
            perror("Seek error");
            free(buffer);
            close(fd);
            return;
        }
        if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
            perror("Write error");
            free(buffer);
            close(fd);
            return;
        }

        // Random read
        if (lseek(fd, offset, SEEK_SET) < 0) {
            perror("Seek error");
            free(buffer);
            close(fd);
            return;
        }
        if (read(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
            perror("Read error");
            free(buffer);
            close(fd);
            return;
        }
    }

    // End timing
    gettimeofday(&end, NULL);

    // Calculate elapsed time in milliseconds
    double elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                             (end.tv_usec - start.tv_usec) / 1000.0;

    // Calculate benchmarking metrics
    double total_data_MB = (double)(NUM_OPERATIONS * BUFFER_SIZE) / (1024 * 1024);
    double throughput_MBps = total_data_MB / (elapsed_time_ms / 1000.0);
    double avg_latency_ms = elapsed_time_ms / (2 * NUM_OPERATIONS);  // 2 operations per loop iteration (1 read, 1 write)
    double iops = (double)(2 * NUM_OPERATIONS) / (elapsed_time_ms / 1000.0);

    // Print results
    printf("Random Access Time: %.2f ms\n", elapsed_time_ms);
    printf("Throughput: %.2f MB/s\n", throughput_MBps);
    printf("Average Latency: %.2f ms\n", avg_latency_ms);
    printf("IOPS: %.2f\n", iops);

    close(fd);
    free(buffer);
}

int main() {
    const char* filename = "random_access_test.bin";

    // Initialize file for testing
    FILE* file = fopen(filename, "wb");
    if (file) {
        char* init_buffer = (char*)malloc(FILE_SIZE);
        if (init_buffer) {
            memset(init_buffer, 'A', FILE_SIZE);
            fwrite(init_buffer, 1, FILE_SIZE, file);
            free(init_buffer);
        }
        fclose(file);
    }

    // Perform random read/write benchmark
    random_read_write(filename);

    return 0;
}
