/**********************************************************
 * Filename: mandelmovie.c
 * Author: Gabi Heuser
 * Project: CPE2600 Lab 11 - Multiprocessing
 * Class-Section: CPE2600-121
 * Purpose: uses forks to produce up to 50 mandel images
 *  gcc -o mandelmovie mandelmovie.c
 **********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CHILDREN 50
#define MAX_THREADS 20

 int main(int argc, char *argv[]){
    
    int num_children = 0;
    int num_threads = 0;
    char c;

    int running = 0;


    //get number of children
    while ((c = getopt(argc, argv, "n:t:"))!=-1){
        if (c == 'n'){
            num_children = atoi(optarg);
        }
        if (c == 't'){
            num_threads = atoi(optarg);
        }
    }

    if (num_children < 1 || num_children > MAX_CHILDREN){
        printf("Error, number of children must be between 1 and 50\n");
        exit(1);
    }
    if (num_threads < 1 || num_threads > MAX_THREADS){
        printf("Error, number of threads must be between 1 and 20\n");
        exit(1);
    }
    printf("Number of children: %d\n", num_children);
    printf("Number of threads: %d\n", num_threads);

    float start_scale = 5;
    float end_scale = .05;
    float scale_step = (start_scale-end_scale)/49;
    for(int i = 0; i < 50; i++){
        //unique filename
        char filename[100];
        snprintf(filename, sizeof filename, "mandel%d.jpg", i);

        char scale[20];
        snprintf(scale, sizeof scale, "%f", start_scale - (i * scale_step));

        char threads[10];
        snprintf(threads, sizeof threads, "%d", num_threads);

        if (running >= num_children){
            wait(NULL);
            running--;
        }

        pid_t pid = fork();
        if(pid < 0){
            perror("fork");
            exit(1);
        }

        if (pid == 0){
            //child
            execl("./mandel", "mandel", "-x", "-1", "-y", "-.1", "-s", scale, "-o", filename, "-t", threads, (char *)NULL);
            perror("execl");
            exit(1);
        }
        //parent falls through to become a new child if needed
        running++;
    }

    while (wait(NULL) > 0){
        //wait until all children catch up
    }
    
    return 0;

 }