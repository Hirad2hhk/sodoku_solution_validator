#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define SIZE 9
#define BLOCK 3

int grid[SIZE][SIZE];
bool valid = true;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define ROW_THREAD 0
#define COL_THREAD 1

typedef struct {
    int id;  
} thread_data_t;

bool is_valid_group(int nums[SIZE]) {
    bool seen[10] = {false};
    
    for (int i = 0; i < SIZE; i++) {
        int n = nums[i];
        
        // Out of range or duplicate?
        if (n < 1 || n > 9 || seen[n]) {
            return false;
        }
        
        seen[n] = true;
    }
    
    return true;
}

void* check_rows(void* arg) {
    int temp[SIZE];
    
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            temp[c] = grid[r][c];
        }
        
        if (!is_valid_group(temp)) {
            pthread_mutex_lock(&lock);
            valid = false;
            pthread_mutex_unlock(&lock);
            printf("Row %d is invalid\n", r + 1);
        }
    }
    
    return NULL;
}

void* check_cols(void* arg) {
    int temp[SIZE];
    
    for (int c = 0; c < SIZE; c++) {
        for (int r = 0; r < SIZE; r++) {
            temp[r] = grid[r][c];
        }
        
        if (!is_valid_group(temp)) {
            pthread_mutex_lock(&lock);
            valid = false;
            pthread_mutex_unlock(&lock);
            printf("Column %d is invalid\n", c + 1);
        }
    }
    
    return NULL;
}

void* check_subgrid(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int grid_id = data->id - 2;  
    
    int start_row = (grid_id / 3) * 3;
    int start_col = (grid_id % 3) * 3;
    
    int temp[SIZE];
    int idx = 0;
    for (int r = 0; r < BLOCK; r++) {
        for (int c = 0; c < BLOCK; c++) {
            temp[idx++] = grid[start_row + r][start_col + c];
        }
    }
    
    if (!is_valid_group(temp)) {
        pthread_mutex_lock(&lock);
        valid = false;
        pthread_mutex_unlock(&lock);
        printf("Subgrid at (%d,%d) is invalid\n", start_row + 1, start_col + 1);
    }
    
    return NULL;
}

void read_grid() {
    printf("Enter your 9x9 Sudoku solution (9 rows of 9 numbers):\n");
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (scanf("%d", &grid[i][j]) != 1) {
                printf("Bad input! Exiting...\n");
                exit(1);
            }
        }
    }
}

int main() {
    read_grid();
    
    pthread_t threads[11];  
    thread_data_t thread_data[11];
    
    for (int i = 0; i < 11; i++) {
        thread_data[i].id = i;
    }
    
    pthread_create(&threads[ROW_THREAD], NULL, check_rows, &thread_data[ROW_THREAD]);
    pthread_create(&threads[COL_THREAD], NULL, check_cols, &thread_data[COL_THREAD]);
    
    for (int i = 0; i < 9; i++) {
        pthread_create(&threads[i+2], NULL, check_subgrid, &thread_data[i+2]);
    }
    
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }
    
    if (valid) {
        printf("VALID SOLUTION!\n");
    } else {
        printf("INVALID SOLUTION!\n");
    }
    
    pthread_mutex_destroy(&lock);
    return 0;
}