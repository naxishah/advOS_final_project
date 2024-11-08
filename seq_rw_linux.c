#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define FILE_SIZE (1024 * 1024 * 10)  // 10 MB
#define BUFFER_SIZE 4096              // 4 KB

void sequential_write(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Failed to open file for writing");
        return;
    }

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Failed to allocate buffer");
        close(fd);
        return;
    }
    memset(buffer, 'W', BUFFER_SIZE);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < FILE_SIZE / BUFFER_SIZE; i++) {
        write(fd, buffer, BUFFER_SIZE);
    }

    gettimeofday(&end, NULL);
    close(fd);
    free(buffer);

    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    double throughput = (FILE_SIZE / (1024.0 * 1024.0)) / (elapsed_time / 1000.0);
    double avg_latency = elapsed_time / (FILE_SIZE / BUFFER_SIZE);
    double iops = (FILE_SIZE / BUFFER_SIZE) / (elapsed_time / 1000.0);

    printf("Sequential Write - Time: %.2f ms, Throughput: %.2f MB/s, Avg Latency: %.2f ms, IOPS: %.2f\n", 
           elapsed_time, throughput, avg_latency, iops);
}

void sequential_read(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file for reading");
        return;
    }

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Failed to allocate buffer");
        close(fd);
        return;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    while (read(fd, buffer, BUFFER_SIZE) > 0);

    gettimeofday(&end, NULL);
    close(fd);
    free(buffer);

    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    double throughput = (FILE_SIZE / (1024.0 * 1024.0)) / (elapsed_time / 1000.0);
    double avg_latency = elapsed_time / (FILE_SIZE / BUFFER_SIZE);
    double iops = (FILE_SIZE / BUFFER_SIZE) / (elapsed_time / 1000.0);

    printf("Sequential Read - Time: %.2f ms, Throughput: %.2f MB/s, Avg Latency: %.2f ms, IOPS: %.2f\n", 
           elapsed_time, throughput, avg_latency, iops);
}

int main() {
    const char *filename = "seq_rw_test.bin";
    sequential_write(filename);
    sequential_read(filename);
    return 0;
}
