# Lab 11 - Multiprocessing
Name: Gabi Heuser \
Class-Section: CPE2600-121

## Overview - `mandelmovie.c`

### Purpose
This code takes the output of mandel and creates 50 images that successively zoom in. 

### Build and Run
1. Use provided Makefile by calling `make` on the command line.\
This will compile the mandel, jpeg, and mandelmovie files. 
2. To run call `./mandelmovie -n <# of processes>` \
This will create 50 images zooming in each time by using the specified number of processes.

### Implementation
Using `getopt` the program can take a command line argument to get the number of processes specified by the user. A loop over 50 frames is then created where a child is forked for each frame and `execl` is used to replace the child with the compiled `mandel` program. A counter keeps track of how many processes are currently running to ensure only the specified number are running at once. A check and wait pause the program if the number of processes is at the threshold until a process finishes. \
This project zooms at the point x = -1, y = -0.1 on a scale from 5 to 0.05 with a linear step between each frame. Every timing run used mandel's default settings (1000 iterations, 1000x1000 pixel images) so the results can be compared fairly. \
The 50 frames were combined into a movie with `ffmpeg -i mandel%d.jpg mandel.mpg`, which is included in this repo as `mandel.mpg`.

## Results

![Runtime vs number of processes](./lab11%20-%20graph.png)

## Analysis
At first, more processes speed up the execution time, but eventually we hit the limit of the number of processors on the machine we are using (in my case 12 processors). This means the runtime stops improving once the number of processes reaches roughly the number of processors. The sharp drop at the beginning shows how using more processes does speed up results when extra processors are available. \
Therefore using more processes does not always speed up the operation because of CPU core limits and the overhead of creating and managing processes.