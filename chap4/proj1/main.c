#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N_THREADS 27

int sudoku[9][9] = {
        { 6, 2, 4, 5, 3, 9, 1, 8, 7 },
        { 5, 1, 9, 7, 2, 8, 6, 3, 4 },
        { 8, 3, 7, 6, 1, 4, 2, 9, 5 },
        { 1, 4, 3, 8, 6, 5, 7, 2, 9 },
        { 9, 5, 8, 2, 4, 7, 3, 6, 1 },
        { 7, 6, 2, 3, 9, 1, 4, 5, 8 },
        { 3, 7, 1, 9, 5, 6, 8, 4, 2 },
        { 4, 9, 6, 1, 8, 2, 5, 7, 3 },
        { 2, 8, 5, 4, 7, 3, 9, 1, 6 }
};

int check_by_thread[N_THREADS];

struct params {
    int row;
    int col;
    int thread_number;
};

void * check_row(void * param) {
    struct params * params = (struct params *) param;
    int row = params->row;
    int arr[10];
    for (int i = 0; i < 10; ++i) {
        arr[i] = 0;
    }
    for (int col = 0; col < 9; ++col) {
        int num = sudoku[row][col];
        if (num < 1 || num > 9 || arr[num] == 1) {
            check_by_thread[params->thread_number] = 1;
        }
        arr[num] = 1;
    }
    free(param);
    pthread_exit(0);
}

void * check_col(void * param) {
    struct params * params = (struct params *) param;
    int col = params->col;
    int arr[10];
    for (int i = 0; i < 10; ++i) {
        arr[i] = 0;
    }
    for (int row = 0; row < 9; ++row) {
        int num = sudoku[row][col];
        if (num < 1 || num > 9 || arr[num] == 1) {
            check_by_thread[params->thread_number] = 1;
            pthread_exit(0);
        }
        arr[num] = 1;
    }
    free(param);
    pthread_exit(0);
}

void * check_square(void * param) {
    struct params * params = (struct params *) param;
    int arr[10];
    for (int i = 0; i < 10; ++i) {
        arr[i] = 0;
    }
    for (int row = params->row; row < params->row + 3; ++row) {
        for (int col = params->col; col < params->col + 3; ++col) {
            int num = sudoku[row][col];
            if (num < 1 || num > 9 || arr[num] == 1) {
                check_by_thread[params->thread_number] = 1;
                pthread_exit(0);
            }
            arr[num] = 1;
        }
    }
    free(param);
    pthread_exit(0);
}

int main() {
    for (int i = 0; i < N_THREADS; ++i) {
        check_by_thread[i] = 0;
    }

    int cur_thread_number = 0;
    pthread_t tids[N_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for (int row = 0; row < 9; ++row) {
        struct params * par = (struct params *) malloc(sizeof(struct params));
        par->row = row;
        par->col = 0;
        par->thread_number = cur_thread_number;
        pthread_create(&tids[cur_thread_number], &attr, check_row, par);
        ++cur_thread_number;
    }
    for (int col = 0; col < 9; ++col) {
        struct params * par = (struct params *) malloc(sizeof(struct params));
        par->row = 0;
        par->col = col;
        par->thread_number = cur_thread_number;
        pthread_create(&tids[cur_thread_number], &attr, check_col, par);
        ++cur_thread_number;
    }
    for (int row = 0; row < 9; row += 3) {
        for (int col = 0; col < 9; col += 3) {
            struct params * par = (struct params *) malloc(sizeof(struct params));
            par->row = row;
            par->col = col;
            par->thread_number = cur_thread_number;
            pthread_create(&tids[cur_thread_number], &attr, check_square, par);
            ++cur_thread_number;
        }
    }
    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(tids[i], NULL);
    }
    for (int i = 0; i < 9; ++i) {
        printf("%d ", check_by_thread[i]);
    }
    printf("\n");
    for (int i = 0; i < 9; ++i) {
        printf("%d ", check_by_thread[i + 9]);
    }
    printf("\n");
    for (int i = 0; i < 9; ++i) {
        printf("%d ", check_by_thread[i + 18]);
    }
    printf("\n");
    for (int i = 0; i < N_THREADS; ++i) {
        if (check_by_thread[i] != 0) {
            printf("invalid");
            return 0;
        }
    }
    printf("valid");
    return 0;
}
