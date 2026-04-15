#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_CONTAINERS 50

typedef enum {
    STARTING,
    RUNNING,
    STOPPED,
    EXITED,
    KILLED
} state_t;

typedef struct {
    char id[32];
    pid_t pid;
    time_t start_time;
    state_t state;
    int soft_limit;
    int hard_limit;
    char log_path[256];
    int exit_code;
    int term_signal;
    int stop_requested;
} container_t;

typedef struct {
    container_t containers[MAX_CONTAINERS];
    pthread_mutex_t lock;
} container_table_t;

// Logging buffer (Task 3)
#define LOG_BUF_SIZE 100

typedef struct {
    char data[LOG_BUF_SIZE][256];
    int in, out, count;
    pthread_mutex_t lock;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} log_buffer_t;

container_table_t table;

// Initialize table
void init_table() {
    pthread_mutex_init(&table.lock, NULL);
}

// Dummy CLI handler (Task 2 placeholder)
void handle_command(char *cmd) {
    printf("Received command: %s\n", cmd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: engine <command>\n");
        return 1;
    }

    init_table();

    if (strcmp(argv[1], "supervisor") == 0) {
        printf("Starting supervisor...\n");
        while (1) {
            sleep(1); // placeholder loop
        }
    }

    // CLI mode
    char command[256] = {0};
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        strcat(command, " ");
    }

    handle_command(command);
    return 0;
}