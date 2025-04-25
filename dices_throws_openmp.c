// gcc -fopenmp dices_throws_openmp.c -o dices_throws_openmp && OMP_NUM_THREADS=12 ./dices_throws_openmp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_THROWS 1000000  // 1 millón de lanzamientos
#define DICE_VALUES 11      // Sumas posibles (2-12)

int main() {
    srand(time(NULL));  // Semilla para números aleatorios
    
    #pragma omp parallel  
    {
        int tid = omp_get_thread_num();     // ID del hilo
        int nthreads = omp_get_num_threads(); // Total de hilos
        int counts[DICE_VALUES] = {0};     // Contador por hilo
        
        // Distribución de trabajo
        int throws = NUM_THROWS / nthreads; // Lanzamientos por hilo
        if (tid < NUM_THROWS % nthreads) throws++; 
        
        // Simulación de lanzamientos
        for (int i = 0; i < throws; i++) {
            int sum = (rand() % 6 + 1) + (rand() % 6 + 1);
            counts[sum-2]++; // Va sumando 1 al contador de la suma que acaba de salir
        }
        
        #pragma omp critical  // Bloque para evitar que los hilos impriman al mismo tiempo y se mezclen los resultados
        {
            printf("\nHilo %d/%d (%d lanzamientos):\n", tid, nthreads, throws);
            for (int s = 0; s < DICE_VALUES; s++) {
                printf("Suma %2d: %6d veces\n", s+2, counts[s]);
            }
        }
    }
    
    return 0;
}