#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define FILE_SIZE 1024 * 1024 * 10  // 10MB file size for testing
#define BUFFER_SIZE 4096            // 4KB buffer size for each read/write operation
#define NUM_OPERATIONS 1000         // Number of random read/write operations

void random_read_write(const char* filename) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("File opening failed");
        return;
    }

    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    memset(buffer, 'R', BUFFER_SIZE);  // Fill buffer with dummy data

    // Start timing
    QueryPerformanceCounter(&start);

    // Perform random read and write operations
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        long offset = rand() % (FILE_SIZE - BUFFER_SIZE);

        // Random write
        fseek(file, offset, SEEK_SET);
        fwrite(buffer, 1, BUFFER_SIZE, file);

        // Random read
        fseek(file, offset, SEEK_SET);
        fread(buffer, 1, BUFFER_SIZE, file);
    }

    QueryPerformanceCounter(&end);

    // Calculate elapsed time
    double elapsed_time_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

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

    fclose(file);
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
