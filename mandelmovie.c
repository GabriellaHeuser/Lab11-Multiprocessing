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

 int main(int argc, char *argv[]){
    
    int num_children = 0;
    char c;

    int running = 0;


    //get number of children
    while ((c = getopt(argc, argv, "n:"))!=-1){
        if (c == 'n'){
            num_children = atoi(optarg);
        }
    }

    if (num_children < 1 || num_children > MAX_CHILDREN){
        printf("Error, number of children must be between 1 and 50\n");
        exit(1);
    }
    printf("Number of children: %d\n", num_children);

    float start_scale = 5;
    float end_scale = .05;
    float scale_step = (start_scale-end_scale)/49;
    for(int i = 0; i < 50; i++){
        //unique filename
        char filename[100];
        snprintf(filename, sizeof filename, "mandel%d.jpg", i);

        char scale[20];
        snprintf(scale, sizeof scale, "%f", start_scale - (i * scale_step));

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
            execl("./mandel", "mandel", "-x", "-1", "-y", "-.1", "-s", scale, "-o", filename, (char *)NULL);
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