// Librerias estándar de C
#include <stdio.h>
#include <stdlib.h>

// Librerias de llamadas al sistema de Unix
#include <unistd.h>
#include <fcntl.h>		
#include <sys/types.h>	
#include <sys/stat.h>	
#include "funciones.h"

/* Le das el puntero a char * IN y te devuelve el caracter hexadecimal correspondiente a 
ese char y al siguiente */


/* Le pasas un array y su longitud y te lo inicializa a 0 */


void zero_char (char * array, int length) {
	int i ;
	for (i = 0; i < length; i++) {
		*(array + i) = 0;
	}
}

/* ---------------------------------------------------------------------------------------------------*/

void zero_int (int * array, int length) {
	int i ;
	for (i = 0; i < length; i++) {
		*(array + i) = 0;
	}
}

/* ---------------------------------------------------------------------------------------------------*/

int str2dec(char  * cad, char digit) {
	int out = 0;
	int i, j;
	int aux;
	
	for (i = 0; i < digit; i++) { 
		aux = cad[i] - '0';
		if((digit -i ) > 0 ) {
			for (j = 1; j < digit - i; j++) {
				aux = aux * 10;
			}
		}
		out += aux;
	}
	return out;
}

/* ---------------------------------------------------------------------------------------------------*/
/* Ordena de mayor a menor un array de int */
int ordenar_int (int * lista, int * orden, int len) {
 
	int aux_big; 
	int aux_pos, aux; 
	int i,j;
	
	for (i = 0; i < len ; i++) {
		orden[i] = i;
	}

	for(i = 0 ;i < len - 1; i++) {
		aux_big = lista[i];
		aux_pos = i;

		for (j = i + 1; j < len; j++) {

			if(aux_big < lista[j]) {
				aux_big = lista [j];
				aux_pos = j;
			}
		 }
		lista[aux_pos] = lista[i];
		lista[i] = aux_big;

		aux = orden[aux_pos];
		orden[aux_pos] = orden[i];
		orden[i] = aux;
	 }
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/
/* Ordena de menor a mayor un array de int */
int ordenar_int2 (int * lista, int * orden, int len) {
 
	int aux_big; 
	int aux_pos, aux; 
	int i,j;
	
	for (i = 0; i < len ; i++) {
		orden[i] = i;
	}

	for(i = 0 ;i < len - 1; i++) {
		aux_big = lista[i];
		aux_pos = i;

		for (j = i + 1; j < len; j++) {

			if(aux_big > lista[j]) {
				aux_big = lista [j];
				aux_pos = j;
			}
		 }
		lista[aux_pos] = lista[i];
		lista[i] = aux_big;

		aux = orden[aux_pos];
		orden[aux_pos] = orden[i];
		orden[i] = aux;
	 }
	return 1;
}
/* Ordena de mayor a menor un array de float	
   orden[i] tiene la posicion dentro del array donde se encuentra la original lista[i] */

int ordenar_float (float * lista, int * orden, int len) {
 
	float aux_big; 
	int aux_pos, aux; 
	int i,j;
	
	for (i = 0; i < len ; i++) {
		orden[i] = i;
	}

	for(i = 0 ;i < len - 1; i++) {
		aux_big = lista[i];
		aux_pos = i;

		for (j = i + 1; j < len; j++) {

			if(aux_big < lista[j]) {
				aux_big = lista [j];
				aux_pos = j;
			}
		 }
		lista[aux_pos] = lista[i];
		lista[i] = aux_big;

		aux = orden[aux_pos];
		orden[aux_pos] = orden[i];
		orden[i] = aux;
	 }
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/
int ordenar_float2 (float * lista, int * orden, int len) {
 
	float aux_big; 
	int aux_pos, aux; 
	int i,j;
	
	for (i = 0; i < len ; i++) {
		orden[i] = i;
	}

	for(i = 0 ;i < len - 1; i++) {

		aux_big = lista[i];
		aux_pos = i;

		for (j = i + 1; j < len; j++) {

			if(aux_big > lista[j]) {
				aux_big = lista [j];
				aux_pos = j;
			}
		 }
		lista[aux_pos] = lista[i];
		lista[i] = aux_big;

		aux = orden[aux_pos];
		orden[aux_pos] = orden[i];
		orden[i] = aux;
	 }
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/
/* Reordena un array tal y como le indica otro */
int reordenar_int (int * lista, int * orden, int len) {
 	int nueva_lista [len];
	int i;
	for(i = 0 ;i < len ; i++) {
		nueva_lista[i] = lista[orden[i]];
	 }
	 copy_vector_int(lista, nueva_lista, len);
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/
/* Reordena un array tal y como le indica otro */
int reordenar_float (float * lista, int * orden, int len) {
 	float nueva_lista [len];
	int i;
	for(i = 0 ;i < len ; i++) {
		nueva_lista[i] = lista[orden[i]];
	 }
	copy_vector_float(lista, nueva_lista, len);
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/
int desreordenar_char (char * lista, int * orden, int len) {
 	char nueva_lista [len];
	int i;
	for(i = 0 ;i < len ; i++) {
		nueva_lista[orden[i]] = lista[i];
	 }
	 copy_vector_char(lista, nueva_lista, len);
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/
/* When we order somethin -> order[i] tell us the object in lista[i]
If we want to know, where is object "i" in lista[i] we use this reverse function where
	order[i] = positions of the object "i" in lista
*/
int reverse_order (int * order2, int * orden, int len) {
 	int nueva_lista [len];
	int i;
	for(i = 0 ;i < len ; i++) {
		nueva_lista[orden[i]] = i;
	 }
	 copy_vector_int(order2, nueva_lista, len);
	return 1;
}

/* ---------------------------------------------------------------------------------------------------*/
int desreordenar_int (int * lista, int * orden, int len) {
 	int nueva_lista [len];
	int i;
	for(i = 0 ;i < len ; i++) {
		nueva_lista[orden[i]] = lista[i];
	 }
	 copy_vector_int(lista, nueva_lista, len);
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/




int dec2str(char  * cad, int digit) {
	int i, j;
	int aux;
	
	i = 0;
	aux = digit;
	while (aux != 0 ) {
		cad[i] = aux % 10 + '0';
		aux = aux / 10;
		i++;
	}
	cad[i] = '\0';
	
	printf("%s \n", cad);
	for (j = 0; j < i/2; j++){
		aux = cad[j];
		cad[j] = cad[i - 1 -j];
		cad[i - 1 -j] = aux;
	}
	printf("%s \n", cad);
	return 1;
}

/* ---------------------------------------------------------------------------------------------------*/


int max(int a, int b) { 
	return (a > b)? a : b;
}

/* ---------------------------------------------------------------------------------------------------*/
int min(int a, int b) { 
	return (a < b)? a : b;
}
/* ---------------------------------------------------------------------------------------------------*/
float maxf (float * array, int num) {
	int i;
	float aux = array[0];

	for (i = 1; i < num; i++) {
		if (array[i] > aux) {
			aux = array[i];
		}
	}
	return aux;
}
/* ---------------------------------------------------------------------------------------------------*/
int findint(int * array, int len, int value) {
	int i;
	for (i = 0; i < len ; i++ ) {
		if (array[i] == value ) {
//			printf("Found on position: %i\n", i);
			return i;
		}
	}
	return -1;  /* Not found */
}

/* ---------------------------------------------------------------------------------------------------*/

int copy_vector_char(char * duplicado, char * original, int num) {
	int i;
	for (i = 0; i < num; i++) {
		duplicado[i] = original[i];
	}
	return 1;
}

/* ---------------------------------------------------------------------------------------------------*/

int copy_vector_int(int * duplicado, int * original, int num) {
	int i;
	for (i = 0; i < num; i++) {
		duplicado[i] = original[i];
	}
	return 1;
}
/* ---------------------------------------------------------------------------------------------------*/

int copy_vector_float(float * duplicado, float * original, int num) {
	int i;
	for (i = 0; i < num; i++) {
		duplicado[i] = original[i];
	}
	return 1;
}

/* ---------------------------------------------------------------------------------------------------*/


int count_char(char * array, int len, int value) {
	int i;
	int count = 0;
	for (i = 0; i < len ; i++ ) {
		if (array[i] == value ) {
//			printf("Found on position: %i\n", i);
			count++;
		}
	}
	return count;  /* Not found */
}
/* ---------------------------------------------------------------------------------------------------*/

int count_int(int * array, int len, int value) {
	int i;
	int count = 0;
	for (i = 0; i < len ; i++ ) {
		if (array[i] == value ) {
//			printf("Found on position: %i\n", i);
			count++;
		}
	}
	return count;  /* Not found */
}

/* ---------------------------------------------------------------------------------------------------*/

int cmp_int(int * array1, int * array2, int len) {
	int i;
	for (i = 0; i < len ; i++ ) {
		if (array1[i] != array2[i] ) {
			return -1;
		}
	}
	return 0;  /* Not found */
}

/* ---------------------------------------------------------------------------------------------------*/


int feas_prune (params * pa, int * num_infeas, int ini) {
	/* Checking that at least there is one person available for every day and if there is only one person
	available the send it to the prunning queue */
	/* The ini variable tell us if its the initial feas check in case the thing is actually imposible */
	int once = 0;	/* We can only add one person to the queue */
	int i,j;
	for (i = 0; i < pa->d; i++) {
		for (j = 1; j < pa->t; j++){	/* pa->table[pa->d][pa->t][0] = No trabajando */
			if (pa->table[i][j].sorted == 0){
				if (pa->table[i][j].people == 0) {
					if ((*num_infeas == 0)||(ini == 1)) {
//					printf("Cagada : Dia %i y tiempo %i  Estado: %i\n", i,j,pa->table[i][j].sorted);
						return -1;	/* Infeasible */
					}
					else {		
					   *num_infeas -= 1;
					   pa->table[i][j].sorted = -1; 
//			 printf("Persona duplicada para: Dia %i y tiempo %i  Estado: %i\n", i,j,pa->table[i][j].sorted);
					pa->solu->time[pa->s].person = -1; 
					pa->solu->time[pa->s].day = i;
					pa->solu->time[pa->s].time = j;
					pa->solu->chose[i*(pa->t -1) + j- 1] = -1;  /* Indicate */
					pa->s += 1;
						/* Means infesable. This way we wont go over it again */
					}
				}
				if (once == 0){
					if (pa->table[i][j].people == 1) {
//						printf("Pruned !!!!!!\n");
						 pa->prune.person = pa->table[i][j].posibles[0];
						 pa->prune.day = i;
						 pa->prune.time = j;
						 once = 1;
						 pa->table[i][j].people -= 1;
					}
				}	
			}
		}
	}
	return once;	/* once = 1 -> prune,   once = 0  -> not prune (take decision)*/
}

int prune (params * pa) {
	/* Prunes the given person and time given in pa->p*/
	/* We can only call this function when we have chosen someone to prune */
	int i,j,k;
	int pos;
	/* Here we can add the constraint of a person not working twice the same day by setting sorted
	both pa->t (morning + afternoon) */
	pa->table[pa->prune.day][pa->prune.time].sorted = 1;

	/* We put it in the solution */
	pa->solu->time[pa->s].person = pa->prune.person;
	pa->solu->time[pa->s].day = pa->prune.day;
	pa->solu->time[pa->s].time = pa->prune.time;
	pa->solu->chose[(pa->prune.day * (pa->t - 1)) + (pa->prune.time  - 1)] = pa->prune.person;  /* Indicate */
	pa->s += 1;		/* Solved one more */
	for (i = 0; i < pa->d; i++) {
		for (j = 1; j < pa->t; j++){	/* pa->table[pa->d][pa->t][0] = No trabajando */
			if (pa->table[i][j].sorted == 0)	{	/* We eliminate that person if it exists */
				pos = findint (pa->table[i][j].posibles, pa->table[i][j].people, pa->prune.person);
				if (pos != -1) {	
					for (k = pos; k < pa->table[i][j].people ; k++) {	
						pa->table[i][j].posibles[k] = pa->table[i][j].posibles[k+1];
						pa->table[i][j].people -= 1;
					}
					
				}
			}
		}
	}
	return 0;
}

				
int take_choice (params * pa, int *n_preferences, pruned * preferences) {
	/* We choose an option that has the minimum posible pa->p */
	int aux_i[10], aux_j[10], num_choices = 0;
	int i,j;
	int aux_d, aux_p, aux_t;
	int choice = 0;
	int randomi, randomi2;
	
	while (choice != 1) {
		if (*n_preferences <= 0) { /* IF there are no preferences */
//			printf("Decision tomada\n");
			/* We create an array with the posible choices we can make */
			for (i = 0; i < pa->d; i++) {
				for (j = 1; j < pa->t; j++){	/* pa->table[pa->d][pa->t][0] = No trabajando */
					if (pa->table[i][j].sorted == 0)	{
	
						aux_i[num_choices] = i;
						aux_j[num_choices] = j;
						num_choices += 1;
					}
				}
			}
			 randomi =  ((rand ()*(float)num_choices/RAND_MAX)); /* Select random time and person */
			 randomi2 = ((rand ()*(float)pa->table[aux_i[randomi]][aux_j[randomi]].people/RAND_MAX));
			
			 pa->prune.person = pa->table[aux_i[randomi]][aux_j[randomi]].posibles[randomi2];
			 pa->prune.day = aux_i[randomi];
			 pa->prune.time = aux_j[randomi];
			 choice = 1;
		}
		else {
			*n_preferences -= 1;
			 /* It could happen that the preference is infeasible itself so we check */
			aux_p = preferences[*n_preferences].person;
			aux_t = preferences[*n_preferences].time;
			aux_d = preferences[*n_preferences].day;

			if (findint ( pa->table[aux_d][aux_t].posibles, pa->table[aux_d][aux_t].people, aux_p)!= -1) { 
//				printf("Preferencia tomada \n");
				 pa->prune.person = aux_p;
				 pa->prune.day = aux_d;
				 pa->prune.time = aux_t;
				 choice = 1;
			}
		}
	}
	return 0;
}

timing ** create_table (int days, int people, int times){
	timing ** table;

	int i, j;
	table = (timing **) malloc (sizeof(timing *) * days );
	if (table == NULL){
		perror ("Error al abrir el archivo");
	}

	for (i = 0; i < days; i++){
		table[i] = (timing *) malloc (times * sizeof (timing));
		if (table[i] == NULL){
			perror ("Error al abrir el archivo");
		}
		for (j = 0; j < times; j++) {	/* Iniciatilize table to 0 */
			table[i][j].posibles = (int *) malloc (people * sizeof(int));
		}
	}
  /* -------------------->  FULLFILL table  <-----------------------------*/
	return table;
}

int init_table (int days, int people, int times, int ** constraints, timing ** table){
	timing * aux_t;
	int i, j;
	for (i = 0; i < days; i++){
		for (j = 0; j < times; j++) {	/* Iniciatilize table to 0 */
			table[i][j].people = 0; /* There are 0 people who can do the work */
			table[i][j].sorted = 0;	/* The place hasnt been sorted */
		}
	}
  /* -------------------->  FULLFILL table  <-----------------------------*/
	for (i = 0; i < people; i++) {
		for (j = 0; j < days; j++) {
			aux_t = &(table[j][constraints[i][j]]);	/* table[j][people[i][j]] = table[day][time] */
			aux_t->posibles[aux_t->people] = i;
			aux_t->people += 1;
		}
	}
	return 1;
}

 
int print_out (sol solution, int people, int days, int times, cadenas cad, timing ** table ) {
	int out_table [people][days];
	int i,j;
	int pers;
	int aux_dia, aux_time;
	for (i = 0; i < people; i++){
		for (j = 0; j < days; j++ ) {
			out_table[i][j] = 0;
		}
	}
	
	for (i = 0; i < 10; i++) {		
		if (solution.time[i].person == -1){	/* The double turn */
			for (j = 0; j < table[solution.time[i].day][solution.time[i].time].people; j++){
				pers = table[solution.time[i].day][solution.time[i].time].posibles[j];
				out_table[pers][solution.time[i].day] = times;	
			}
		}
	}
	for (i = 0; i < 10; i++) {
		if (solution.time[i].person != -1){	
			out_table[solution.time[i].person][solution.time[i].day] = solution.time[i].time;
		}
	}
	
	for (i = 0; i < people; i++) {
		printf(" %s \t",cad.people[i]);

		for (j = 0; j < days; j++) {
			printf ("%s \t", cad.times[out_table[i][j]]);
		}
		printf("\n");
	}
	printf("Numero de preferencias conseguidas --> %i\n", solution.preferences);

	printf("Codigo de la solucion: ");
	for (i = 0; i < days * (times-1);i++){
		printf("%i ",solution.chose[i]);
	}
	printf("\n");
	printf("Posibles Dobles turnos: \n");
	for (i = 0; i < 10; i++) {
		if (solution.chose[i] == -1 ) {
			aux_dia = i/(times -1);
			aux_time = i % (times -1);
		        printf("%s %s ---> ",cad.days[aux_dia],cad.times[aux_time + 1]);
			for (j = 0; j < table[aux_dia][aux_time + 1].people; j++) {
				printf("%s ", cad.people[table[aux_dia][aux_time+ 1].posibles[j]]);
			}
			printf("\n");
		}
	}
	
	return 1;

}

int permutate_pref (pruned * preferences, int n_preferences){
	pruned aux[n_preferences];
	int randomi;
	int taken[n_preferences];
	int i, j, ok;
	for (i = 0; i < n_preferences; i++){
		taken[i] = -1;
	}

	for (j = 0; j < n_preferences; j++) {
		ok = 0;
		while (ok != 1){
			ok = 1;
			randomi = ((rand ()*(float)n_preferences/RAND_MAX));
			for (i = 0; i < n_preferences; i++) {
				if (randomi == taken[i]){
					ok = 0;
				}	
			}
		}
		taken[j] = randomi;
	}
/*	for (i = 0; i < n_preferences; i++){
		printf("%i ", taken[i]);
	}
	printf("\n");
*/
	for (i = 0; i < n_preferences; i++){
		aux[i] = preferences[taken[i]];
	}
	for (i = 0; i < n_preferences; i++){
		preferences[i] = aux[i];
	}

	return 1;
}



int check_solution (sol * solutions, int n_solution, int days, int times) {
/* It checks if we already have that solution */
	int i;
	if (n_solution > 0) {
//	printf("Doing \n");
		for ( i = 0; i < n_solution ; i++) {
			if (cmp_int (solutions[i].chose, solutions[n_solution].chose, days*(times-1)) == 0 ){
				return -1;	/* We already had this solution */
			}
		}
	}

	return 0;		/* We didnt have this solution */
}

int print_top_sol (sol solution, int w, int days, int times, cadenas cad, int * people_week, int ** pref_week) {
	int i,j,k;
	int pers;
	int hole;
	int aux_holes;
	hole = 0;

	j = 0;

	for (i = 0; i < (times-1)*days; i++) {
		if (solution.chose[i] != -1){	
			printf("%i ",solution.chose[i]);
			printf(" %s \t",cad.total_people[solution.chose[i]]);
		
			if (i >=times -1) {

				for (j = 0; j < i/(times -1); j++) {
					printf ("%s \t", cad.times[0]);
				}
			}
			printf ("%s \t", cad.times[i%(times-1) + 1]);
			for (k = j; k < days - 1; k++) {
				printf ("%s \t", cad.times[0]);
			}
			printf("\n");
		}
	}
	printf("Numero de preferencias conseguidas --> %i\n", solution.preferences);

	printf("Codigo de la solucion: ");
	for (i = 0; i < days * (times-1);i++){
		printf("%i ",solution.chose[i]);
	}
	printf("\n");
	printf("Posibles Dobles turnos: \n");

	hole = -1;
	aux_holes =  (days)*(times - 1) - min( people_week[w],(days)*(times - 1) );
	for (i = 0; i < ((times-1)*days - people_week[w]); i++) {	/* For every hole */
		hole = 	findint (solution.chose + hole + 1,((times-1)*days) - hole, -1) + hole + 1;
		 printf("%s %s ---> ",cad.days[hole/(times-1)],cad.times[hole%(times-1) + 1]);
//		printf("dfe %i",solution.posibles[i][0]);
		for (j = 0; j < solution.posibles[i][0]; j++) {
			pers = solution.posibles[i][j+1];
			printf("%s ", cad.total_people[pers]);	
		}
		printf("\n");
	}
	printf("Preferences: \n");
	for (i = 0; i < (times-1)*days; i++) {
		if (pref_week[w][i] != -1){	
			printf(" %s ", cad.total_people[pref_week[w][i]]);
		}
	}
	printf("\n");
	return 1;

}


int permutate_weeks (int weeks, int * top_week, int * week_order, int * top_order){
	int randomi;
	int taken[100];
	int i, j, ok;
	for (i = 0; i < weeks; i++){
		taken[i] = -1;
	}

	for (j = 0; j < weeks; j++) {
		ok = 0;
		while (ok != 1){
			ok = 1;
			randomi = ((rand ()*(float)weeks/RAND_MAX));
			for (i = 0; i < weeks; i++) {
				if (randomi == taken[i]){
					ok = 0;
				}	
			}
		}
		week_order[j] = randomi;
		taken[j] = randomi;
	}
	for ( i = 0; i < weeks; i++) {
		randomi = ((rand ()*(float)top_week[week_order[i]]/RAND_MAX));
		top_order[i] = randomi;
	}

/*	for (i = 0; i < weeks; i++){
		printf("%i -> %i \t ", week_order[i],top_order[i] );
	}
*/

	return 1;
}

float get_variance (int * times_worked, int total_people) {
	float average = 0.0;
	float variance = 0.0;
	int i;
	int actual_people = 0;
//	for (i = 0; i < 
	for (i = 0; i< total_people; i++){
		if (times_worked[i] > 0) {	/* Coz if it s 0 this person doesnt work */
			average += times_worked[i];
			actual_people += 1;
		}
	}
	average = average / actual_people;
	
	for (i = 0; i< total_people; i++){
		if (times_worked[i] > 0) {	/* Coz if it s 0 this person doesnt work */
			variance += (times_worked[i] - average)*(times_worked[i] - average);
		}
	}
	variance = variance / actual_people;
	return variance;
}

int out_sol (char * fichero , sol2 gold, int weeks, int days, int times, cadenas cad, int * people_week, sol ** total_solutions, int ** pref_week, int total_people) {
	int hole;
	int aux_holes;
	int filled;
	int orden[weeks];
	int i,w;
	int aux_chose[days*(times -1)];
	sol aux_sol;
	int times_work[total_people];

	int out_table [total_people][days];
	int j;

	FILE *pf;

	pf = fopen(fichero, "w+");
/*
	for (i = 0; i < weeks; i++ ) {
		printf("%i ", gold.week_order[i]);
	}
	printf("\n");

	for (i = 0; i < weeks; i++ ) {
		printf("%i ", gold.top_order[i]);
	}
	printf("\n");
*/
	printf("\n");
	desreordenar_int (gold.top_order, gold.week_order, weeks);
	ordenar_int2 (gold.week_order, orden, weeks); /* To print them in order */
//	reordenar_int (gold.top_order, orden, weeks);
	printf("Preferencias conseguidas %i, varianza %f\n", gold.preferences, gold.variance);

/*
	for (i = 0; i < weeks; i++ ) {
		printf("%i ", gold.week_order[i]);
	}
	printf("\n");

	for (i = 0; i < weeks; i++ ) {
		printf("%i ", gold.top_order[i]);
	}
	printf("\n");
*/
	for (i = 0; i < total_people; i++) {
		times_work[i] = 0;
	}
//
	for (w = 0; w < weeks; w++){
		 hole = -1;
		 aux_sol = total_solutions[gold.week_order[w]][gold.top_order[w]];
		 copy_vector_int(aux_chose, aux_sol.chose, (times-1)*days);
		 aux_holes =  (days)*(times - 1) - min(people_week[gold.week_order[w]],(days)*(times - 1) );
		 for ( filled = 0; filled <  aux_holes  ; filled ++) {
			hole = 	findint (aux_chose + hole +1,((times-1)*days) - hole, -1) + hole + 1;
			aux_chose[hole] = gold.more_turn[orden[w]][filled];
//		printf("<%i> ",gold.more_turn[orden[w]][filled]);
		}
		for ( i = 0; i < (times-1)*days; i++) {
			printf("%i ", aux_chose[i]);
			times_work[aux_chose[i]] += 1;
		}
		printf("\n");
		/* AUX_CHOSE CONTAINS ALL THE INFO NEEDED TO PRINTF THE SOLUTION OF A GIVEN WEEK */


	
		for (i = 0; i < total_people; i++){
			for (j = 0; j < days; j++ ) {
				out_table[i][j] = 0;
			}
		}
		for (i = 0; i < days*(times-1); i++) {	
			out_table[aux_chose[i]][i/(times -1)] = i%(times-1)+1;
		}
//		printf("*********** Semana %i ***************\n", w+1);
		fprintf(pf, "*********** Semana %i ***************\n", w+1);
		for (i = 0; i < total_people; i++) {
//			printf(" %s \t",cad.total_people[i]);
			fprintf(pf," %s \t",cad.total_people[i]);
			for (j = 0; j < days; j++) {
//				printf ("%s \t", cad.times[out_table[i][j]]);
				fprintf (pf, "%s \t", cad.times[out_table[i][j]]);
			}
//			printf("\n");
			fprintf(pf,"\n");
			
		}
//		printf("\n");
		fprintf(pf,"\n");
	}
//	printf("DATOS SOBRE EL HORARIO: \n");
//	printf("Media de preferencias satisfechas por semana %f \n", (float)gold.preferences/weeks );
//	printf("Varianza de numero de horas de trabajadores activos %f\n", gold.variance);
//	printf("Numero de horas que trabaja cada persona: \n");
	fprintf(pf,"DATOS SOBRE EL HORARIO: \n");
	fprintf(pf,"Media de preferencias satisfechas por semana %f \n", (float)gold.preferences/weeks );
	fprintf(pf,"Varianza del numero de horas %f\n", gold.variance);
	fprintf(pf,"Numero de turnos que trabaja cada persona: \n");
	for (i = 0; i < total_people; i++) {
//		printf("%s --> %i \n",cad.total_people[i], times_work[i]);
		fprintf(pf,"%s --> %i \n",cad.total_people[i], times_work[i]);
	}
//	fprintf(pf,"\n\n\n");
	fclose(pf);
	
	return 2;
}
/* We are gonna reorder the weeks so that we can print them in order */

int check_solution2 (sol2 * solutions, int conf, int days, int times, int weeks) {
/* It checks if we already have that solution */
/* 2 solutions are equal if they have choosen the same double turns and same top weeks.
First, we order the top weeks and cheek it, then for each top week we check the chosen people.
We are gonna consider them equal if they have:
	- Same chosen top weeks
	- Same preferences
	- Same variance
 */

	int i;
	int aux_top_order[weeks];
	int aux_top_order2[weeks];



	copy_vector_int (aux_top_order, solutions[conf].top_order, weeks);
	desreordenar_int (aux_top_order, solutions[conf].week_order, weeks);

	if (conf > 0) {
//	printf("Doing \n");
		for ( i = 0; i < conf; i++) {
			copy_vector_int (aux_top_order2, solutions[i].top_order, weeks);
			desreordenar_int (aux_top_order2, solutions[i].week_order, weeks);

			if (cmp_int (aux_top_order2, aux_top_order, weeks) == 0 ){
				if (solutions[i].variance == solutions[conf].variance) {
					if (solutions[i].preferences == solutions[conf].preferences) {
						return -1;	/* We already had this solution */
					}
				}
			}
		}
	


	}	

	return 0;		/* We didnt have this solution */
}
	




