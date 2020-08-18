#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n_samples = 1000000000;
int n_hit = 0;

void * runner(void * param) {
    srand(0);
    for (int i = 0; i < n_samples; ++i) {
        double sample_x = (double) rand() / RAND_MAX * 2 - 1;
        double sample_y = (double) rand() / RAND_MAX * 2 - 1;
        if (sample_x * sample_x + sample_y * sample_y <= 1) {
            ++n_hit;
        }
    }
    pthread_exit(0);
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid, &attr, runner, NULL);

    pthread_join(tid, NULL);

    double pi_appr = 4.0 * n_hit / n_samples;
    printf("Pi approximation is %.6f\n", pi_appr);

    return 0;
}
