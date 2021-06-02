/*
 -------------------------------------
 File:    z_creator.c
 Project: 190906250_190765210_a01_q01
 -------------------------------------
 Author:  Nausher Rao & Declan Hollingworth
 ID:      190906250 & 190765210
 Email:   raox6250@mylaurier.ca & holl5210@mylaurier.ca
 Version  2021-06-02
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    // Forks the parent process.
    pid_t pid = fork();

    // Child process
    if (pid == 0)
    {
        printf("\nChild");
        exit(0);
    }
    // Parent process
    else if (pid > 0)
    {
        // Sleeps for 100s.
        printf("\nParent");
        sleep(100);
    }
    else
    {
        printf("\nError!");
        return 1;
    }

    return 0;
}
