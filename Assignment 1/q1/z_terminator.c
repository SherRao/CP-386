/*
 -------------------------------------
 File:    z_terminator.c
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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    // Run the first program.
    system("./z_creator.exe &");

    // Print initial list.
    printf("\n\nInitial list of processes:\n\n");
    system("ps -l");
    sleep(2);

    // Kill parent process.
    system("kill -9 $(ps -l|grep -w Z|tr -s ' '|cut -d ' ' -f 5)");
    sleep(2);

    // Print final list.
    printf("\n\nAfter killing the zombie proccess, new list of processes:\n\n");
    system("ps -l");

    return 0;
}