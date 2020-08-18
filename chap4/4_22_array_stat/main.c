#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double avg;
int min;
int max;

struct number_array_struct {
    int * arr;
    size_t size;
};

void * count_avg(void * param) {
    struct number_array_struct * number_array = (struct number_array_struct *) param;
    int sum = 0;
    for (int i = 0; i != number_array->size; ++i) {
        sum += number_array->arr[i];
    }
    avg = (double) sum / number_array->size;
    pthread_exit(0);
}

void * count_min(void * param) {
    struct number_array_struct * number_array = (struct number_array_struct *) param;
    min = number_array->size != 0 ? number_array->arr[0] : 0;
    for (int i = 1; i < number_array->size; ++i) {
        int cur = number_array->arr[i];
        if (cur < min) {
            min = cur;
        }
    }
    pthread_exit(0);
}

void * count_max(void * param) {
    struct number_array_struct * number_array = (struct number_array_struct *) param;
    max = number_array->size != 0 ? number_array->arr[0] : 0;
    for (int i = 1; i < number_array->size; ++i) {
        int cur = number_array->arr[i];
        if (cur > max) {
            max = cur;
        }
    }
    pthread_exit(0);
}

int main(int argc, char ** argv, char ** envp) {
    int numbers[argc - 1];
    for (int i = 1; i != argc; ++i) {
        numbers[i - 1] = atoi(argv[i]);
    }

    struct number_array_struct number_array = { numbers, argc - 1 };

    pthread_t avg_tid;
    pthread_t min_tid;
    pthread_t max_tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    if (pthread_create(&avg_tid, &attr, count_avg, &number_array) != 0) {
        printf("Error creating a thread\n");
        return 1;
    }
    if (pthread_create(&min_tid, &attr, count_min, &number_array) != 0) {
        printf("Error creating a thread\n");
        return 1;
    }
    if (pthread_create(&max_tid, &attr, count_max, &number_array) != 0) {
        printf("Error creating a thread\n");
        return 1;
    }
    pthread_join(avg_tid, NULL);
    pthread_join(min_tid, NULL);
    pthread_join(max_tid, NULL);

    printf("The average value is %.2f\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);

    return 0;
}
