#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define FILE_SIZE 1024 * 1024 * 10  // 10MB file size for testing
#define BUFFER_SIZE 4096            // 4KB buffer size for each read/write operation

void sequential_write(const char* filename) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    FILE* file = fopen(filename, "wb");
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

    memset(buffer, 'W', BUFFER_SIZE);  // Fill buffer with dummy data for writing

    QueryPerformanceCounter(&start);

    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++) {
        fwrite(buffer, 1, BUFFER_SIZE, file);
    }

    QueryPerformanceCounter(&end);

    double elapsed_time_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    double total_data_MB = (double)FILE_SIZE / (1024 * 1024);
    double throughput_MBps = total_data_MB / (elapsed_time_ms / 1000.0);
    double avg_latency_ms = elapsed_time_ms / (FILE_SIZE / BUFFER_SIZE);
    double iops = (double)(FILE_SIZE / BUFFER_SIZE) / (elapsed_time_ms / 1000.0);

    printf("Sequential Write Time: %.2f ms\n", elapsed_time_ms);
    printf("Throughput: %.2f MB/s\n", throughput_MBps);
    printf("Average Latency: %.2f ms\n", avg_latency_ms);
    printf("IOPS: %.2f\n", iops);

    fclose(file);
    free(buffer);
}

void sequential_read(const char* filename) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    FILE* file = fopen(filename, "rb");
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

    QueryPerformanceCounter(&start);

    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++) {
        fread(buffer, 1, BUFFER_SIZE, file);
    }

    QueryPerformanceCounter(&end);

    double elapsed_time_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    double total_data_MB = (double)FILE_SIZE / (1024 * 1024);
    double throughput_MBps = total_data_MB / (elapsed_time_ms / 1000.0);
    double avg_latency_ms = elapsed_time_ms / (FILE_SIZE / BUFFER_SIZE);
    double iops = (double)(FILE_SIZE / BUFFER_SIZE) / (elapsed_time_ms / 1000.0);

    printf("Sequential Read Time: %.2f ms\n", elapsed_time_ms);
    printf("Throughput: %.2f MB/s\n", throughput_MBps);
    printf("Average Latency: %.2f ms\n", avg_latency_ms);
    printf("IOPS: %.2f\n", iops);

    fclose(file);
    free(buffer);
}

int main() {
    const char* filename = "seq_rw_test.bin";

    // Perform sequential write and read tests
    sequential_write(filename);
    sequential_read(filename);

    return 0;
}
