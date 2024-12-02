#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define FILE_SIZE 1024 * 1024 * 10  // 10MB file size for testing
#define BUFFER_SIZE 4096            // 4KB buffer size for each read/write operation

void sequential_write(const char* filename) {
    struct timeval start, end;

    // Open file for writing
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
    memset(buffer, 'W', BUFFER_SIZE);  // Fill buffer with dummy data for writing

    // Start timing
    gettimeofday(&start, NULL);

    // Perform sequential write operations
    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++) {
        if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
            perror("Write error");
            free(buffer);
            close(fd);
            return;
        }
    }

    // End timing
    gettimeofday(&end, NULL);

    // Calculate elapsed time
    double elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                             (end.tv_usec - start.tv_usec) / 1000.0;

    // Calculate benchmarking metrics
    double total_data_MB = (double)FILE_SIZE / (1024 * 1024);
    double throughput_MBps = total_data_MB / (elapsed_time_ms / 1000.0);
    double avg_latency_ms = elapsed_time_ms / (FILE_SIZE / BUFFER_SIZE);
    double iops = (double)(FILE_SIZE / BUFFER_SIZE) / (elapsed_time_ms / 1000.0);

    // Print results
    printf("Sequential Write Time: %.2f ms\n", elapsed_time_ms);
    printf("Throughput: %.2f MB/s\n", throughput_MBps);
    printf("Average Latency: %.2f ms\n", avg_latency_ms);
    printf("IOPS: %.2f\n", iops);

    close(fd);
    free(buffer);
}

void sequential_read(const char* filename) {
    struct timeval start, end;

    // Open file for reading
    int fd = open(filename, O_RDONLY);
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

    // Start timing
    gettimeofday(&start, NULL);

    // Perform sequential read operations
    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++) {
        if (read(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
            perror("Read error");
            free(buffer);
            close(fd);
            return;
        }
    }

    // End timing
    gettimeofday(&end, NULL);

    // Calculate elapsed time
    double elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                             (end.tv_usec - start.tv_usec) / 1000.0;

    // Calculate benchmarking metrics
    double total_data_MB = (double)FILE_SIZE / (1024 * 1024);
    double throughput_MBps = total_data_MB / (elapsed_time_ms / 1000.0);
    double avg_latency_ms = elapsed_time_ms / (FILE_SIZE / BUFFER_SIZE);
    double iops = (double)(FILE_SIZE / BUFFER_SIZE) / (elapsed_time_ms / 1000.0);

    // Print results
    printf("Sequential Read Time: %.2f ms\n", elapsed_time_ms);
    printf("Throughput: %.2f MB/s\n", throughput_MBps);
    printf("Average Latency: %.2f ms\n", avg_latency_ms);
    printf("IOPS: %.2f\n", iops);

    close(fd);
    free(buffer);
}

int main() {
    const char* filename = "seq_rw_test.bin";

    // Perform sequential write and read tests
    sequential_write(filename);
    sequential_read(filename);

    return 0;
}
