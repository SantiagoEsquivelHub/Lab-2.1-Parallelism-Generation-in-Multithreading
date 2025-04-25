/*
 gcc -fopenmp parallel_sum_openmp.c -o parallel_sum_openmp 
OMP_NUM_THREADS=4 ./parallel_sum_openmp 4
OMP_NUM_THREADS=1 ./parallel_sum_openmp 8
OMP_NUM_THREADS=2 ./parallel_sum_openmp 6 
OMP_THREAD_LIMIT=2 OMP_NUM_THREADS=8 ./parallel_sum_openmp 8
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_INTEGERS 100000000LL

// Version 1: Usando OMP_NUM_THREADS
void version1() {
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (long long i = 1; i <= MAX_INTEGERS; i++) {
        sum += i;
    }
    printf("Version 1 (OMP_NUM_THREADS): sum = %lld\n", sum);
}

// Version 2: Usando omp_set_num_threads()
void version2(int num_threads) {
    omp_set_num_threads(num_threads);
    long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (long long i = 1; i <= MAX_INTEGERS; i++) {
        sum += i;
    }
    printf("Version 2 (omp_set_num_threads(%d)): sum = %lld\n", num_threads, sum);
}

// Version 3: Usando num_threads()
void version3(int num_threads) {
    long long sum = 0;
    #pragma omp parallel for num_threads(num_threads) reduction(+:sum)
    for (long long i = 1; i <= MAX_INTEGERS; i++) {
        sum += i;
    }
    printf("Version 3 (num_threads(%d)): sum = %lld\n", num_threads, sum);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }
    int num_threads = atoi(argv[1]);

    double start, end;

    // Version 1
    start = omp_get_wtime();
    version1();
    end = omp_get_wtime();
    printf("Time: %.4f seconds\n\n", end - start);

    // Version 2
    start = omp_get_wtime();
    version2(num_threads);
    end = omp_get_wtime();
    printf("Time: %.4f seconds\n\n", end - start);

    // Version 3
    start = omp_get_wtime();
    version3(num_threads);
    end = omp_get_wtime();
    printf("Time: %.4f seconds\n", end - start);

    return 0;
}