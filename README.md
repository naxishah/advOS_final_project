# Cross-Platform File System Performance Evaluation

## Overview
This project evaluates the performance of different file systems on Windows and Linux through sequential and random read/write tests. The primary goal is to understand how each file system handles various data access patterns by collecting key performance metrics such as throughput, latency, and IOPS. By comparing default file systems (NTFS, ext4, XFS) and NTFS on Linux (using NTFS-3G), this study provides actionable insights into the strengths and weaknesses of each system across different platforms.

## Features
- **Sequential and Random Read/Write Testing**: Custom C programs measure performance on specific access patterns.
- **Cross-Platform Testing**: Replicate tests across Windows and Linux file systems.
- **Benchmarking Tools**: Integrates `fio` and `dd` for automated, advanced benchmarking and analysis.
- **NTFS-3G Implementation**: Tests NTFS on Linux to facilitate direct comparisons with native NTFS on Windows.

## Requirements
- **Operating Systems**: Windows 11, Ubuntu 20.04 (or newer)
- **Software**: 
  - `gcc` for compiling C code on Linux
  - Visual Studio (or `cl` compiler) for Windows
  - `ntfs-3g`, `fio`, and `dd` for Linux benchmarking
- **Hardware**: Preferably a dual-boot system or virtual machines for cross-platform testing

## Install Required Software
### On Windows:
- Ensure you have Visual Studio or cl for compiling C code.
### On Ubuntu:
```bash
sudo apt update
sudo apt install gcc fio ntfs-3g
```
## Compile the C Programs
### On Windows:
- cl seq_rw.c random_rw.c
### On Ubuntu:
- gcc -o seq_rw seq_rw.c
- gcc -o random_rw random_rw.c

## Running fio Benchmarks
### On Windows:
- .\seq_rw.exe
- .\random_rw.exe
### On Ubuntu:
- ./seq_rw
- ./random_rw

## Running KDiskMark Benchmarks
1. Install and run program 
```bash
sudo apt update
sudo apt install kdiskmark
```
From terminal launch 
```bash
kdiskmark
```
2. Configure the Test:
- Select the partition you want to test (e.g., /mnt/windows for NTFS or /dev/sdaX for ext4).
- Set test parameters (same as Windows for consistency):
-   File size: 1 GB or 10 GB.
-   Test iterations: Default (usually 5).
-   Block sizes: Include common sizes (e.g., 4 KB, 64 KB, 1 MB).
3. Start the Test:
- Click "Start Benchmark" to run the sequential and random read/write tests.
- Once complete, export or record the results.
4. Log Results:
- Use the "Export" option to save the results as a text or image file.
- Record metrics like Throughput (MB/s) and IOPS for each test.
