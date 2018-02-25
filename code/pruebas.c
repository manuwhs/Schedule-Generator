// A Dynamic Programming based solution for 0-1 Knapsack problem
#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "BB.h"
#include "DP.h"
 
 
int main() {
	
   int ** po;
   int i,j;
   int capacity = 100000;
   int items = 9000;

	po = (int ** ) malloc (items* sizeof(int * ));
 	for (i = 0; i < items; i++){
		po[i] = (int * ) malloc (capacity * sizeof(int));
		if (po[i] == NULL) {
			perror("Error de Memoria");
			break;
		}
	}
	printf("La hemos cagado a la %i \n", i + 1);
	printf ("Memoria reservada: %i MBytes\n", i * sizeof(int) * capacity/1000000);
	
	for (j = 0; j < i ; j++){
		free(po[i]);
	}
	free (po);
	printf("Memoria liberada %i \n", j + 1);
}
    



