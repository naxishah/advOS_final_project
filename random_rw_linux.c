#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define FILE_SIZE (1024 * 1024 * 10)  // 10 MB
#define BUFFER_SIZE 4096              // 4 KB
#define NUM_OPERATIONS 1000           // Number of random operations

void random_read_write(const char *filename) {
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("Failed to open file");
        return;
    }

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (!buffer) {
        perror("Failed to allocate buffer");
        close(fd);
        return;
    }
    memset(buffer, 'R', BUFFER_SIZE);

    struct timeval start, end;
    srand(time(NULL));

    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_OPERATIONS; i++) {
        off_t offset = (rand() % (FILE_SIZE / BUFFER_SIZE)) * BUFFER_SIZE;
        lseek(fd, offset, SEEK_SET);
        write(fd, buffer, BUFFER_SIZE);

        lseek(fd, offset, SEEK_SET);
        read(fd, buffer, BUFFER_SIZE);
    }

    gettimeofday(&end, NULL);
    close(fd);
    free(buffer);

    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    double throughput = (NUM_OPERATIONS * BUFFER_SIZE / (1024.0 * 1024.0)) / (elapsed_time / 1000.0);
    double avg_latency = elapsed_time / (2 * NUM_OPERATIONS);  // 2 operations per loop
    double iops = (2 * NUM_OPERATIONS) / (elapsed_time / 1000.0);

    printf("Random Access - Time: %.2f ms, Throughput: %.2f MB/s, Avg Latency: %.2f ms, IOPS: %.2f\n", 
           elapsed_time, throughput, avg_latency, iops);
}

int main() {
    const char *filename = "random_rw_test.bin";
    random_read_write(filename);
    return 0;
}
