# Lab 11/12 - Multiprocessing and Multithreading

Name: Gabi Heuser

Class-Section: CPE2600-121

## Overview - `mandelmovie.c`

### Purpose
This code takes the output of mandel and creates 50 images that successively zoom in. The time it takes to run depends on the number of processes and threads decided by the user.

### Build and Run
1. Use the provided Makefile by calling `make` on the command line. This will compile the mandel, jpeg, and mandelmovie files.
2. To run, call `./mandelmovie -n <# of processes> -t <# of threads>`. This will create 50 images zooming in each time by using the specified number of processes and threads.

### Implementation
Using `getopt` the program can take a command line argument to get the number of processes specified by the user. A loop over 50 frames is then created where a child is forked for each frame and `execl` is used to replace the child with the compiled `mandel` program. A counter keeps track of how many processes are currently running to ensure only the specified number are running at once. A check and wait pause the program if the number of processes is at the threshold until a process finishes.

Within the mandel image computation, pthreads are used to generate the individual images. The number of threads is determined by the user at the command line and can be between 1 and 20. The image is split into horizontal bands of rows, and each thread computes the image data for its own band. The last thread also takes any leftover rows when the image height is not evenly divisible by the thread count. Each thread is given its inputs through a struct, and all threads are joined before the image is saved as a whole.

This project zooms at the point x = -1, y = -0.1 on a scale from 5 to 0.05 with a linear step between each frame. Every timing run used mandel's default settings (1000 iterations, 1000x1000 pixel images) so the results can be compared fairly.

The 50 frames were combined into a movie with `ffmpeg -i mandel%d.jpg mandel.mpg`, which is included in this repo as `mandel.mpg`.

## Results

![Runtime vs number of processes](./lab11%20-%20graph.png)

Graph from Lab 11 does not include thread data.

| Threads \ Processes | 1 | 2 | 5 | 10 | 20 | 50 |
|---|---|---|---|---|---|---|
| 1 | 161.880s | 88.141s | 52.400s | 38.802s | 35.375s | 34.209s |
| 2 | 94.871s | 63.692s | 37.739s | 36.705s | 35.841s | 37.528s |
| 5 | 66.713s | 56.885s | 35.371s | 34.007s | 36.612s | 35.359s |
| 10 | 46.523s | 44.382s | 34.232s | 36.128s | 36.846s | 35.182s |
| 20 | 43.068s | 42.720s | 33.242s | 33.975s | 35.398s | 36.777s |

## Analysis

### Runtime vs # Processes
At first, more processes speed up the execution time, but eventually we hit the limit of the number of processors on the machine we are using (in my case 14 processors). This means the runtime stops improving once the number of processes reaches roughly the number of processors. The sharp drop at the beginning shows how using more processes does speed up results when extra processors are available.

Therefore using more processes does not always speed up the operation because of CPU core limits and the overhead of creating and managing processes.

### Threads and Processes
As more threads and processes are used, the time for execution decreases. This makes sense. As numbers get very high, the payoff decreases since the machine runs out of open cores to run on and the operating system instead must allocate within them and they will operate below 100% to attempt to get the proper number of processes or threads to run at the same time. This means that an optimal amount of threads and processes lies somewhere that the