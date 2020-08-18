#include <stdio.h>
#include <stdlib.h>

int n_samples_per_thread = 10000000;
int n_samples = 0;
int n_hit = 0;

/*
 * comple with -fopenmp flag:
 * gcc -fopenmp main.c
 */
int main() {

    srand(0);

    #pragma omp parallel
    {
        for (int i = 0; i < n_samples_per_thread; ++i) {
            double sample_x = (double) rand() / RAND_MAX * 2 - 1;
            double sample_y = (double) rand() / RAND_MAX * 2 - 1;
            ++n_samples;
            if (sample_x * sample_x + sample_y * sample_y <= 1) {
                ++n_hit;
            }
        }
    }

    double pi_appr = 4.0 * n_hit / n_samples;
    printf("Pi approximation is %.6f\n", pi_appr);

    return 0;
}
