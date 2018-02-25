#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"
#include <math.h>

int main(int argc, char **argv) {
    int max_char = 30;	/* Maximum number of characters a word can have */
    int days;		/* Days for the time table */
    int times;		/* Different periods of time at which a person can work */
    int people;		/* Number of people */

    cadenas cad;

    char ** days_s;
    char ** times_s;
    char ** people_s;	/* Where the people's name will be put */
    int ** constraints; 	/* contraints[people][days] will give me the constraints */

    char aux[30];
    FILE *pf;
    sol * solutions;	/* solutions[person][days] = time */
    int n_solution = 0;	/* Number of solutions found so far */
    int current_try = 0;
    char * fichero;
    int i,j,k;			/*Contadores */

    timing ** table;	 /* Table[days][times] = People that work that day at that time */
    timing ** table_aux;  /* Table to be constrained */
    int make_prune = 0;	/* indicates whether there is prunning ready to do or not */

    params pa;		/* Parameters structure */

    pruned * preferences;	/* Whenever we have to take a choice, we first choose one of those,
				It will be a queue of the "timing" preferencias */
    int n_preferences = 0;
    int n_pref;

    char infeasible_s[10] = {"xxxxxx"};	/* Means infeasible */ 
    int num_infeas;	/* Number of possible infeasibilities */

    int tries = 10000;	/*  NUMBER OF SOLUTIONS WE ARE GONNA TRY */
    int top = 5;	/* TOP 10 solutions. They are better if they acomplish more preferences and
			more people can swap jobs */
    int n_top = 0;	/* Actual number of top solutions found */
    sol * top_solutions ;  /* Top solutions */

    int * top_pos;	/* To get the positions of the best solutions */
    int * order;

/* Every person should only work at least one every week */
/* There has to be one person working in the morning and other one in the afternoon (not the same)*/

    /* We load the timetable constraints from the file */

	if (argc < 2) {
		printf("Faltan archivo de entrada \n");
		return -1;
	}
	fichero = argv[1];
	pf = fopen(fichero, "r+");	/* Openning file */
	if (pf == NULL) {
		perror ("Error al reservar memoria");
	}
/* -------------------->  Reading days  <-----------------------------*/
	fscanf (pf, "%i", &days);	

	days_s = (char **)malloc (days * sizeof(char *));
	if (days_s == NULL) {
		perror ("Error al reservar memoria");
	}

	for (i = 0; i < days; i++) {
		days_s[i] = (char *)malloc (max_char * sizeof(char));
		if (days_s[i] == NULL) {
			perror ("Error al reservar memoria");
		}
		fscanf(pf, "%s", days_s[i]);
	}

	
/* -------------------->  Reading times <-----------------------------*/

	fscanf (pf, "%i", &times);
	times_s = (char **)malloc ((times+1) * sizeof(char *)); /* +1 for the infeasible part */
	if (times_s == NULL) {
		perror ("Error al reservar memoria");
	}

	for (i = 0; i < times; i++) {
		times_s[i] = (char *)malloc (max_char * sizeof(char));
		if (times_s[i] == NULL) {
			perror ("Error al reservar memoria");
		}
		fscanf(pf, "%s", times_s[i]);
	}
	times_s[times] = infeasible_s;

/* -------------------->  Reading people and their constraints <-----------------------------*/

	fscanf (pf, "%i", &people);
	people_s = (char **)malloc (people * sizeof(char *));
        constraints = (int **) malloc (people * sizeof(int *));

	if ((people_s == NULL)||(constraints == NULL)) {
		perror ("Error al reservar memoria");
	}

	for (i = 0; i < people; i++) {
		people_s[i] = (char *)malloc (max_char * sizeof(char));
		constraints[i] = (int *) malloc (days * sizeof(int));

		if ((people_s[i] == NULL)||(constraints[i] == NULL)) {
			perror ("Error al reservar memoria");
		}

		fscanf(pf, "%s", people_s[i]);

		for (j = 0; j < days; j++) {
			fscanf(pf, "%s", aux);
			
			for ( k = 0; k < times; k++) {
				if (strcmp(aux, times_s[k]) == 0 ) {
					constraints[i][j] = k;
				}
			}
		}
	}
/* -------------------->  Read preferences<-----------------------------*/
	preferences = (pruned *) malloc ( 10 * sizeof(pruned));
	for (j = 1; j < times; j++) {
		for (i = 0; i < days; i++) {

			fscanf(pf, "%s", aux);
				for (k = 0; k < people; k++) {
					if (strcmp(aux, people_s[k]) == 0){
						preferences[n_preferences].day = i;
						preferences[n_preferences].time = j;
						preferences[n_preferences].person = k;
						n_preferences += 1;
					}
				}
		}
	}
	fclose(pf);

/*
	for (i = 0; i < n_preferences; i++) {
	printf(" %s \t %s \t %s \n", people_s[preferences[i].person], days_s[preferences[i].day], times_s[preferences[i].time]);
	}
*/
/* Making names length stardad */
	for (i = 0; i < people; i++) {
		for (j = strlen(people_s[i]); j < 10; j++ ) {
			people_s[i][j] = ' ';
		}
		people_s[i][10] = 0;
	}
/* -------------------->  PRINT CONSTRAINTS <-----------------------------*/
/*
	for (i = 0; i < people; i++) {
		printf(" %s \t",people_s[i]);

		for (j = 0; j < days; j++) {
			printf ("%s \t", times_s[constraints[i][j]]);
		}
		printf("\n");
	}

	printf("Solution leida \n");
*/

/* -------------------->  CREATE SOLUTION STRUCTURE  <-----------------------------*/

	solutions = (sol *) malloc (tries * sizeof (sol));  /* "tries" posible solutions at most */

	if (solutions == NULL){
		perror ("Error al reservar memoria");
	}
	for (i = 0; i < tries; i++ ) {
		solutions[i].chose = (int *) malloc (((times-1)*days)*sizeof(int));
		if (solutions[i].chose == NULL){
			perror ("Error al reservar memoria");
		}
	}
	/* At every try to find a solution, if we find it, we save it in one of these, if not, reuse it */

	top_solutions = (sol *) malloc (top * sizeof (sol)); 
	if (top_solutions == NULL){
		perror ("Error al reservar memoria");
	}

/* -------------------->  CREATE TABLE  <-----------------------------*/

	table =  create_table (days, people, times);
	table_aux = create_table (days, people, times);

	init_table (days, people, times,constraints, table);
	init_table (days, people, times, constraints, table_aux);

  /* --------------------->  Print Table <---------------------------------- */

/*
	for (i = 0; i < days; i++) {
		printf ("%s: \n", days_s[i]);
		for (j = 1; j < times; j++) {		
			printf(" %s \t", times_s[j]);
			for( k = 0; k < table[i][j].people; k++) {	
				printf(" %s  | ", people_s[table[i][j].posibles[k]]);
			}
			printf("\n");
		}
	}
*/
printf("Table created \n");
/* --------------------> INITIALICE PARAMS STRUCTURE <-----------------------------*/

    pa.p = people;
    pa.t = times;
    pa.d = days;
    pa.table = table_aux;
    pa.s = 0;		/* Solved */
    pa.solu = solutions;

    num_infeas = 10 - people;	/* Number of posible infeasibilities */
    n_pref = n_preferences;


    cad.people = people_s;
    cad.days = days_s;
    cad.times = times_s;
    current_try = 0;

printf("Parameters initialiced \n");

/* First we check initial feasinility and prunning */



/* --------------------> REAL DEAL !!!!<-----------------------------*/

while (current_try < tries ) {	/* While we havent found 10 solution */

	init_table (days, people, times, constraints, table_aux);	/* Reset constraints */
    	pa.table = table_aux;
	pa.solu = solutions + n_solution;		/* New solution */
	pa.solu->preferences = 0;
	n_preferences = n_pref;
//	printf("gververv e %i \n",n_preferences);
	num_infeas = 10 - people;
	pa.s = 0;
	permutate_pref (preferences, n_pref);		/* Reset the way we make chioces */
/*
	for (i = 0; i < n_preferences; i++){
		printf("%i ",preferences[i].day);
	}
	printf("\n");
*/
	make_prune = feas_prune (&pa,&num_infeas,1);
	if (make_prune == -1){ 
		printf ("Infeasibe \n");
		exit(-1);
	}

   while (pa.s < 10) {	/* Because 10 are the places we have to fill */

	if (make_prune == 0 ) {
		take_choice (&pa, &n_preferences, preferences);
//		printf("Choice taken: \n");
	}
// 	printf("Pruning: %s  -> %s -> %s \n", people_s[pa.prune.person],days_s[pa.prune.day], times_s[pa.prune.time]);
	prune (&pa);
	make_prune = feas_prune (&pa,&num_infeas, 0);
	if (make_prune == -1){ 
//		printf ("Infeasibe --------------------------------------->\n");
		break;
	}

   }
   if (make_prune != -1) { /* If it was feasible we count it as a solution */
	if (check_solution (solutions, n_solution, days, times)== 0) {			/* If we hadnt found that solution before */

	for (i = 0; i < n_pref; i++) {  /* In this loop we get the number of preferences right */
	if ( solutions[n_solution].chose[preferences[i].day * (times - 1) + preferences[i].time - 1] == preferences[i].person ) {
			solutions[n_solution].preferences += 1;
			}
		}
		n_solution += 1;	
		printf("Solution %i \n", n_solution);

	}
   }
   current_try += 1;	/* Try again */
}
printf("ALL POSSIBLE SOLUTIONS FOUND: %i\n", n_solution);

/* -------------------->  GET BEST SOLUTIONS <-----------------------------*/ 
/* The way we are gonna find them is slow but efective. 
Our priority is to find the ones with the most preferences posible so we order them in decreasing
amount of preferences succeded*/
	top_pos = (int *) malloc (n_solution * sizeof(int));
	order = (int *) malloc (n_solution * sizeof(int));
	if ((top_pos == NULL)||(order == NULL)) {
		perror("Error de memoria\n");
	}

	for (i = 0; i < n_solution; i++){
		top_pos[i] = solutions[i].preferences;
	}
	ordenar_int (top_pos, order, n_solution);

	n_top = min(top, n_solution);
	for ( i = 0; i < n_top; i++){
		top_solutions [i] = solutions[order[i]];
	}
/* -------------------->  PRINT Solution <-----------------------------*/
	if (n_solution == 0) {
		printf("Ninguna solucion es posible, alguien no trabajaria \n");
	}
	for (i = 0; i < n_top; i++){
		print_out (top_solutions[i], people, days, times, cad, table );
		printf("\n");
	}

  /* -------------------->  Free Memory <-----------------------------*/

	for (i = 0; i < days; i++) {
		free (days_s[i]);
		for (j = 0; j < times; j++) {
			free (table[i][j].posibles);
			free (table_aux[i][j].posibles);
		}	
		free (table[i]);
		free (table_aux[i]);
	}
	free (days_s);
	free (table);
	free (table_aux);

	for (i = 0; i < times; i++) {
		free (times_s[i]);	
	}
	free (times_s);

	for (i = 0; i < people; i++) {
		free (people_s[i]);
		free (constraints[i]);	
	}
	free (people_s);
	free (constraints);	
	free (preferences);
	
	for (i = 0; i < tries; i++) {
		free (solutions[i].chose);
	}

	free (solutions);
	free (top_solutions);
	free (top_pos);
	free (order);
/* Contraints */
	printf("Memoria liberada con exito \n");
    return 1; 
}










