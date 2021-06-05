#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]);
int get_new_memory_id();
char *get_memory_pointer(int memory_id);
void write_file_to_memory(char *memory_pointer, char *fileName, int length);
void exec_commands_from_memory(char *memory_pointer);
void write_command_output_to_pipe(char *pointer, int pipe_id);
void pipe_to_file(int pipe_id);

const int SHARED_MEMORY_SIZE = 4096;
const char *SHARED_MEMORY_FILE_NAME = "PROC_SHM";
const char *OUTPUT_FILE_NAME = "output.txt";

int main(int argc, char *args[]) {
    if (argc > 1) {
        int memory_id = get_new_memory_id();
        char *memory_pointer = get_memory_pointer(memory_id);

        char *fileName = args[1];
        write_file_to_memory(memory_pointer, fileName, strlen(fileName));
        exec_commands_from_memory(memory_pointer);
    }

    else {
        printf("\nmain: No file given!\n");
        exit(-1);
    }
}

int get_new_memory_id() {
    int shared_mem_id =
        shm_open(SHARED_MEMORY_FILE_NAME, O_CREAT | O_RDWR, 0666);
    if (shared_mem_id == -1) {
        printf("\nget_new_memory_id: Shared memory failed: %s\n",
               strerror(errno));
        exit(-1);
    }

    ftruncate(shared_mem_id, SHARED_MEMORY_SIZE);
    return shared_mem_id;
}

char *get_memory_pointer(int shared_mem_id) {
    char *pointer = mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE,
                         MAP_SHARED, shared_mem_id, 0);
    if (pointer == MAP_FAILED) {
        printf("\nget_memory_pointer: Map failed: %s\n", strerror(errno));
        exit(-1);
    }

    return pointer;
}

void write_file_to_memory(char *memory_pointer, char *fileName, int length) {
    char *pointer = memory_pointer;
    pid_t pid = fork();

    if (pid == 0) {
        FILE *file = fopen(fileName, "r");
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, file)) != -1) {
            pointer += sprintf(pointer, "%s", line);
        }

        exit(0);
    }

    else if (pid > 0) {
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1) {
            perror("\nwrite_file_to_memory: Error while waiting\n");
            exit(-1);
        }

        printf("\nwrite_file_to_memory: Done waitin\n");
    }

    else {
        printf("\nwrite_file_to_memory: Error while forking!\n");
        exit(-1);
    }
}

void exec_commands_from_memory(char *memory_pointer) {
    char *pointer = memory_pointer;

    char *pipe_name = "/tmp/mypipe";
    mkfifo(pipe_name, 0666);
    pid_t pid = fork();

    if (pid == 0) {
        int pipe_id = open(pipe_name, O_WRONLY);
        char result[SHARED_MEMORY_SIZE];
        for (int i = 0; i < 64; i += 1) {
            char data = (char)pointer[i];
            strncat(result, &data, 1);
        }

        char *result_pointer = strtok(result, "\r\n");
        write_command_output_to_pipe(result_pointer, pipe_id);
        close(pipe_id);
    }

    else if (pid > 0) {
        int pipe_id = open(pipe_name, O_RDONLY);
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1) {
            perror("\nexec_commands_from_memory: Error while waiting\n");
            exit(-1);
        }

        pipe_to_file(pipe_id);
        close(pipe_id);
    }

    else {
        printf("\nexec_commands_from_memory: Error while forking!\n");
        exit(-1);
    }
}

void write_command_output_to_pipe(char *pointer, int pipe_id) {
    char result[SHARED_MEMORY_SIZE];
    while (pointer) {
        FILE *virtual_file = popen(pointer, "r");
        char line[1035];
        if (virtual_file) {
            char first_line[50];
            sprintf(first_line, "The output of: %s : is\n>>>>>>>>>>>>>>>\n", pointer);
            strcat(result, first_line);

            while (fgets(line, sizeof(line), virtual_file) != NULL) {
                strcat(line, "\n");
                strcat(result, line);
            }

            strcat(result, "<<<<<<<<<<<<<<<");

        } else {
            printf("\nexecute_commands: Error while executing '%s'!\n",
                   pointer);
            exit(-1);
        }

        fclose(virtual_file);
        pointer = strtok(NULL, "\r\n");
    }

    write(pipe_id, result, SHARED_MEMORY_SIZE + 1);
}

void pipe_to_file(int pipe_id) {
    char result[SHARED_MEMORY_SIZE];
    read(pipe_id, result, SHARED_MEMORY_SIZE);

    FILE *output_file = fopen(OUTPUT_FILE_NAME, "w");
    char *pointer = strtok(result, "\r\n");
    while (pointer) {
        fprintf(output_file, "%s\n", pointer);
        pointer = strtok(NULL, "\r\n");
    }

    fclose(output_file);
}