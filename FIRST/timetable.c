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
    int aleat;		/* Random number */
    int aux_double = 0;
    int aux_days_row = 0;
    int days_row;

    cadenas cad;
    int ok;
    char ** days_s;
    char ** times_s;
    char ** total_people_s;	/* Where all the people's name will be put */
    char ** people_s;		/* Where the people's name at every week will be put */
    int ** constraints; 	/* contraints[people][days] will give me the constraints */

    char aux[50];
    FILE *pf;
    sol * solutions;	/* solutions[person][days] = time */
    int n_solution = 0;	/* Number of solutions found so far */
    int current_try = 0;
    char * fichero;
    int i,j,k,w;			/*Contadores */
    int aux_i;

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
    sol aux_sol;
    int * top_pos;	/* To get the positions of the best solutions */
    int * order;
    int weeks;		/* Number of weeks */
    int total_people;
    int *real_people;	/* Array which tells us in a given week, which person is the one in each constraint */
    int *times_worked;	/* Number of ours everyone has worked */

    sol ** total_solutions;	/* For every week there will be an array of their top solutions 
				This variable has all those arrays */
    int * people_week;	/* Array which tell us the number of people at every week */
    int ** pref_week;	/* Array of the preferences of every week. The number of preferences is 10 (days*(times-1)) */
			/* When there is not a preference, it will have value -1 */
    int * top_week;	/* Array with the number of top_solutions found at every week */
    int hole = 0;
    timing pos_double;

    int * week_order;
    int * top_order;
    int * order_hours;
    int * doubled;
    int conf;
    int max_confs = 100;
    sol2 * final_solutions;	/* Final solutions structure */
    int filled;
    char salida[20];
    int max_solutions = 100;		/* Maximum number of solutions per week */
    int aux_holes;
    int aux_holes2;
    int feas_conf = 0;
    int tries_conf = 200000000;
    int * order_aux;
    float * variances;			/* To write all the variances of the solutions */
    int * top_conf_order;
    int n_double;			/* Number of double turns in two weeks in a row */
/* Every person should only work at least one every week */
/* There has to be one person working in the morning and other one in the afternoon (not the same)*/

    /* We load the timetable constraints from the file */

	if (argc < 3) {
		printf("Faltan archivos de entrada \n");
		return -1;
	}
	fichero = argv[1];
	pf = fopen(fichero, "r+");	/* Openning file */
	if (pf == NULL) {
		perror ("Error al abrir el archivo");
	}
/* -------------------->  Reading parameters <-----------------------------*/
fscanf (pf, "%s", aux);
fscanf (pf, "%i", &tries_conf);	

fscanf (pf, "%s", aux);
fscanf (pf, "%i", &max_confs);	

fscanf (pf, "%s", aux);
fscanf (pf, "%i", &aleat);	
srand(aleat);

fscanf (pf, "%s", aux);
fscanf (pf, "%i", &n_double);	

fscanf (pf, "%s", aux);
fscanf (pf, "%i", &days_row);	

fscanf (pf, "%s", aux);
fscanf (pf, "%i", &top);


/* -------------------->  Reading days  <-----------------------------*/
	fscanf (pf, "%s", aux);
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
	fscanf (pf, "%s", aux);
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

/* -------------------->  Reading all the people  <-----------------------------*/
	fscanf (pf, "%s", aux);
	fscanf (pf, "%i", &total_people);
	total_people_s = (char **)malloc (total_people * sizeof(char *));

	if (total_people_s == NULL) {
		perror ("Error al reservar memoria");
	}

	for (i = 0; i < total_people; i++) {
		total_people_s[i] = (char *)malloc (max_char * sizeof(char));

		if (total_people_s[i] == NULL) {
			perror ("Error al reservar memoria");
		}
		fscanf(pf, "%s", total_people_s[i]);
	}

/* -------------------->  Reading weeks <-----------------------------*/
	fscanf (pf, "%s", aux);
	fscanf (pf, "%i", &weeks);

/* For every week we are gonna calculate the "top" best schedules, one we do that, we only keep the solution
which will tell everything we need to know about the week.
We can reuse the "table" and "preferences" for every week */

/* -------------------->  Create the data structures that will be used by all weeks<-----------------------------*/

	preferences = (pruned *) malloc ( days * (times-1) * sizeof(pruned));
	if (preferences == NULL) {
		perror ("Error al reservar memoria");
	}

	table =  create_table (days, total_people, times);
	table_aux = create_table (days, total_people, times);

	solutions = (sol *) malloc (max_solutions * sizeof (sol));  /* "tries" posible solutions at most */

	if (solutions == NULL){
		perror ("Error al reservar memoria");
	}
	for (i = 0; i < max_solutions; i++ ) {
		solutions[i].chose = (int *) malloc (((times-1)*days)*sizeof(int));
		if (solutions[i].chose == NULL){
			perror ("Error al reservar memoria");
		}
	}
	
	total_solutions = (sol **) malloc ( weeks * sizeof(sol *));
	if ( total_solutions== NULL) {
		perror ("Error al reservar memoria");
	}
	people_week = (int *) malloc ( sizeof(int) * weeks );
	if (people_week == NULL) {
		perror ("Error al reservar memoria");
	}
	top_week = (int *) malloc ( sizeof(int) * weeks );
	if (top_week == NULL) {
		perror ("Error al reservar memoria");
	}
	pref_week = (int **) malloc (sizeof (int *) * weeks );
	if (pref_week == NULL) {
		perror ("Error al reservar memoria");
	}

	cad.total_people = total_people_s;

/* --------------------------------------------------------------------------------------------------------------*/
/* ------------------------------> SOLVING EVERY WEEK SEPARATELLY <----------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------*/

printf("Commun data structures for all weeks created\n");
for (w = 0; w < weeks; w++) {

	fscanf (pf, "%s", aux);
	fscanf (pf, "%i", &people);
printf("*************************************************************************************\n");
printf("People in week %i -> %i \n", w + 1, people);
printf("*************************************************************************************\n");

	/* Set the things that are commun to every week */
	n_preferences = 0;
	n_solution = 0;
	current_try = 0;
	make_prune = 0;
	num_infeas =  (days)*(times - 1) - min( people,(days)*(times - 1) );;

	people_week[w] = people;

/* -------------------->  Reading people and their constraints localy  <-----------------------------*/

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
			ok = 0;
			for ( k = 1; k < times; k++) {
				if (strcmp(aux, times_s[k]) == 0 ) {
					constraints[i][j] = k;
					ok = 1;
					break;
				}
			}
			if (ok == 0) {
				constraints[i][j] = 0;
			}
		}
	}

       real_people = (int*)malloc (sizeof(int)*people);		/* Position the people has in the global people */
	if (real_people == NULL) {
		perror ("Error al reservar memoria");
	}

	for (i = 0; i < people; i++) {
		for ( j = 0; j < total_people; j++) {
			if (strcmp(people_s[i], total_people_s[j]) == 0 ) {
				real_people[i] = j;
			}
		}
	}

printf("People and their constrains ready \n");

/* -------------------->  Read preferences<-----------------------------*/
	for (j = 1; j < times; j++) {	/* j = 1 coz the 0 value is for the "not available" */
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
	
	pref_week[w] = (int *) malloc (days * (times - 1)*sizeof(int));
	if (pref_week[w] == NULL) {
		perror ("Error al reservar memoria");
	}

	for (i = 0; i < days * (times - 1);  i++){
		pref_week[w][i] = -1;
	}
	for (i = 0; i < n_preferences;  i++){	
		pref_week[w][(preferences[i].day * (times - 1)) + (preferences[i].time  - 1)] = preferences[i].person;
	}

printf("Preferences ready \n");

/* -------------------->  CREATE TOP SOLUTION STRUCTURE  <-----------------------------*/
	
/* This is the only data structure we are keeping from every week
We have to create a new one every week and dont "free it" */

	top_solutions = (sol *) malloc (top * sizeof (sol)); 
	if (top_solutions == NULL){
		perror ("Error al reservar memoria");
	}

	total_solutions[w] = top_solutions;	

printf("Top solutions structure ready \n");

/* -------------------->  FORM TABLES  <-----------------------------*/

	init_table (days, people, times,constraints, table);
	init_table (days, people, times, constraints, table_aux);

printf("Table ready \n");

  /* --------------------->  Print DATA of the WEEK <---------------------------------- */


/*
	for (i = 0; i < n_preferences; i++) {
	printf(" %s \t %s \t %s \n", people_s[preferences[i].person], days_s[preferences[i].day], times_s[preferences[i].time]);
	}
*/

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

printf("Table created \n");
*/

/* --------------------> INITIALICE PARAMS STRUCTURE <-----------------------------*/

    pa.p = people;
    pa.t = times;
    pa.d = days;
    pa.table = table_aux;
    pa.s = 0;		/* Solved */
    pa.solu = solutions;

    n_pref = n_preferences;

    cad.people = people_s;
    cad.days = days_s;
    cad.times = times_s;
    current_try = 0;

printf("Parameters initialiced \n");

/* First we check initial feasinility and prunning */



/* --------------------> REAL DEAL !!!!<-----------------------------*/

while ((current_try < tries)&&(n_solution < max_solutions) ) {	/* While we havent found 10 solution */

	init_table (days, people, times, constraints, table_aux);	/* Reset constraints */
    	pa.table = table_aux;
	pa.solu = solutions + n_solution;		/* New solution */
	pa.solu->preferences = 0;
	n_preferences = n_pref;
	num_infeas = aux_holes =  (days)*(times - 1) - min( people ,(days)*(times - 1) );;
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

   while (pa.s < days * (times -1)) {	/* Because 10 are the places we have to fill */

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
//		printf("Solution %i \n", n_solution);

	}
   }
   current_try += 1;	/* Try again */
}

printf("ALL POSSIBLE SOLUTIONS FOUND: %i\n", n_solution);

/* Making names length stardad */
	for (i = 0; i < people; i++) {
		for (j = strlen(people_s[i]); j < 10; j++ ) {
			people_s[i][j] = ' ';
		}
		people_s[i][10] = 0;
	}


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
	top_week[w] = n_top;
	for ( i = 0; i < n_top; i++){
		top_solutions [i] = solutions[order[i]];
	}
	printf("Got the best %i solutions\n", n_top);

/* IMPORTANT CONCEPT */
/* When we asign one structure to another:	top_solutions [i] = solutions[order[i]],
all the parameters of the right-handside structure are copied into the left hand side one,
for the arrays, only the initial pointer is copied, not the array itself so if we "free" or change that
array from "solutions[order[i]]", it will also change in "top_solutions [i]", and since we are using
the same array "solutions" at every week what we will do for every top solution of every week is:
	1- Allocate independent memory for them and make the initial asign top_solutions [i] = solutions[order[i]]
	    This will copy normal variables good and pointers.
	2 - Allocate memory for the arrays in the "sol" structure and copy the arrays.
This way, the arrays in "solutions" and in "top_solutions" will be totally indepent and we free them separatelly
and can change "solutions" from week to week without altering the previous top solutions */

/* Notice that:
	- At the top_solutions, the parameter time of the "sol" structure is totally useless, we dont need to
	allocate memory for it coz with the parameter "choice" we have all thar info-
	- At the top_solutions we have to create the array with the people who can do "double turn" and 
	their turn so that we know how to asign the people
	- For every week, we have to create an array with their preferences because when asigning the "double turn",
	it may happen that more preferences are satisfied. This can happen because initially, since a requirement
	is that everyone can only work once and every one has to work once.
*/
	for (i = 0; i < n_top; i++) {
		top_solutions[i].chose = (int *) malloc ( sizeof(int)*((times-1)*days));
		aux_holes =  (days)*(times - 1) - min( people,(days)*(times - 1) );
		top_solutions[i].posibles = (int **) malloc (sizeof(int*) * aux_holes);
		if (top_solutions[i].chose== NULL) {
			perror ("Error al reservar memoria");
		}
		if (top_solutions[i].posibles == NULL) {
			perror ("Error al reservar memoria");
		}


		copy_vector_int(top_solutions[i].chose, solutions[order[i]].chose, ((times-1)*days));
//		printf("Choses copies\n");
		hole = -1;
/*
		for (j = 0; j < ((times-1)*days); j++) {
			printf("%i ", top_solutions[i].chose[j]);
		}
		printf("\n");
*/
/* For every posible double_turn, the position top_solutions[i].posibles[j][0] has the number of people that can
make the double turn that day */
		aux_holes =  (days)*(times - 1) - min( people,(days)*(times - 1) );
		for (j = 0; j < aux_holes; j++) {
			hole = 	findint (top_solutions[i].chose + hole +1,((times-1)*days) - hole, -1) + hole + 1;
			pos_double = table[hole/(times-1)][hole%(times-1)+1];
			top_solutions[i].posibles[j] = (int *) malloc (sizeof(int)*(pos_double.people + 1));
			if (top_solutions[i].posibles[j] == NULL) {
				perror ("Error al reservar memoria");
			}
			top_solutions[i].posibles[j][0] = pos_double.people;
			for (k = 1; k < pos_double.people + 1; k++) {
				 top_solutions[i].posibles[j][k] = real_people[pos_double.posibles[k-1]];
//				printf(" %s \n", total_people_s[top_solutions[i].posibles[j][k]]);
			/* Notice the real_people thing so that we directly have the absolut names */
			}
		}
	}
	

/* -------------------->  REWRITE the top solutions with their overall number of the people <-----------------------------*/
/* For the "chose" */
	for (i = 0; i < n_top; i++ ) {
		for (j = 0; j < (times-1)*days; j++) {
			if ( top_solutions [i].chose[j] != -1){
				top_solutions [i].chose[j] = real_people[top_solutions [i].chose[j]];
			}
		}
	}
/* For the "preferences" */
	for (j = 0; j < (times-1)*days; j++) {
		if ( pref_week[w][j] != -1){
			pref_week[w][j] = real_people[pref_week[w][j]];
		}
	}

/* -------------------->  REWRITE the top solutions with their real number and create  <-----------------------------*/
/* -------------------->  PRINT Solution <-----------------------------*/
	if (n_solution == 0) {
		printf("Ninguna solucion es posible, alguien no trabajaria en la semana %i \n",w);
		return 1;
	}
/*
	for (i = 0; i < n_top; i++){
		print_out (top_solutions[i], people, days, times, cad, table );
		printf("\n");
	}
*/
  /* -------------------->  Free Local Memory used for every week<-----------------------------*/

	for (i = 0; i < people; i++) {
		free (people_s[i]);
		free (constraints[i]);	
	}
	free (people_s);
	free (constraints);


}	/* CLOSES THE LOOP OF THE WEEKS */

/* Free memory we wont use again */
	for (i = 0; i < max_solutions; i++ ) {
		free(solutions[i].chose);
	}
	free (solutions);
	free (preferences);


printf("****************************************************************************\n");
printf("********************* ALL WEEKS SUCCEDED INDIVIDUALLY **********************\n");
printf("****************************************************************************\n");
/* Making names length stardad */
	for (i = 0; i < total_people; i++) {
		for (j = strlen(total_people_s[i]); j < 10; j++ ) {
			total_people_s[i][j] = ' ';
		}
		total_people_s[i][10] = 0;
	}
/* PRINT GLOBAL TOP SOLUTIONS TO SEE IF THEY MATCH THE LOCAL ONES */
/*	for ( w = 0; w < weeks; w++){
		for (i = 0; i < top_week[w]; i++){
		
			print_top_sol (total_solutions[w][i], w, days, times, cad, people_week, pref_week);

		}
	}
*/
  /* -------------------->  TIME FOR GETTING ALL WEEKS SORTED <-----------------------------*/
  /* Simple idea:
      Use constraint programming again with, all weeks, what we will do is:
	- Having an array with the number of ours everybody has worked.
	- We make work double to someone who has least hours worked, we can choose among them
	  the one that satisfies a preference.
	- Keep doing that to every hole of every week.
	- We cannot make someone work 3 times in a week
	Notice that a person cannot work more than one turn in one day but of course this constraint
	is already satisfied by the input itself so no need to check it.

     We do this permutating the weeks and the order in which we fill the weeks.
     At the end we will have many posibilities. We choose the one with the most preferences succeded
     and most regular hours worked. */

	times_worked = (int *)malloc  ( sizeof(int) * total_people );
	if (times_worked  == NULL) {
		perror ("Error al reservar memoria");
	}
	week_order = (int *)malloc  ( sizeof(int) * weeks );	
	if (week_order  == NULL) {
		perror ("Error al reservar memoria");
	}
	order_aux = (int *)malloc  ( sizeof(int) * weeks );	
	if (order_aux  == NULL) {
		perror ("Error al reservar memoria");
	}
	top_order = (int *)malloc  ( sizeof(int) * weeks );	
	if ( week_order == NULL) {
		perror ("Error al reservar memoria");
	}
 	order_hours = (int *) malloc (sizeof(int)*total_people);
	if (order_hours  == NULL) {
		perror ("Error al reservar memoria");
	}
	doubled = (int *) malloc (sizeof(int)*total_people);	/* Auxiliary variable to put the people that already
									is doing double turn in the same week */
	if (doubled == NULL) {
		perror ("Error al reservar memoria");
	}
/* Picking some schedule from the top_schedules of every week and the order we are gonna process them 
	We need two random int arrays: 
	1- For random order in which we will process the weeks
	2- Random top_schdule we are gonna choose at every week*/



	final_solutions = (sol2 *) malloc (sizeof(sol2)*max_confs);
	if (final_solutions == NULL) {
		perror ("Error al reservar memoria");
	}

        conf = 0;
	current_try = 0;

while ((current_try < tries_conf)&&(conf < max_confs)) {	/* We try "tries" configurations of weeks */
	feas_conf = 1;
// printf(" %i / %i\n",conf,current_try );
/* Things we are gonna use at every try and we have to reinitialice if we change them:
	- Number of initially worked hours 
	- The posible people that can do "double turn" at every week.
	- The number of preferences at every week.
	- Solution: Never change anything exept the hours worked
*/

    permutate_weeks ( weeks, top_week,  week_order, top_order);

/* Initial number of ours everyone has worked */
	for (i = 0; i < total_people; i++){
		times_worked[i] = 0;
	}
    /* Get the initial number of ours worked by anybody */
	for (i = 0; i < weeks; i++) {
		for (j = 0; j < (days* (times-1)); j++) {
			if (total_solutions[week_order[i]][top_order[i]].chose[j] != -1){
				times_worked[total_solutions[week_order[i]][top_order[i]].chose[j]]+= 1;
			}
		}
	}
//	ordenar_int2 (times_worked, order_hours, total_people);

/* Get initial preferences satisfied */
	final_solutions[conf].preferences = 0;
	for (i = 0; i < weeks; i++) {
		final_solutions[conf].preferences += total_solutions[week_order[i]][top_order[i]].preferences;
			
	}

/* Allocate memory for the final solutions */

	final_solutions[conf].week_order = (int *) malloc (sizeof(int)*weeks);
	if (final_solutions[conf].week_order == NULL) {
		perror ("Error al reservar memoria");
	}
	final_solutions[conf].top_order = (int *) malloc (sizeof(int)*weeks);
	if (final_solutions[conf].top_order == NULL) {
		perror ("Error al reservar memoria");
	}
	final_solutions[conf].more_turn = (int **) malloc (sizeof(int * )*weeks);
	if (final_solutions[conf].more_turn == NULL) {
		perror ("Error al reservar memoria");
	}
	for (i = 0; i < weeks; i++) {
		aux_holes =  (days)*(times - 1) - min( people_week[week_order[i]],(days)*(times - 1) );
		final_solutions[conf].more_turn[i] = (int *) malloc (sizeof(int)*aux_holes);
		if (final_solutions[conf].more_turn[i] == NULL) {
			perror ("Error al reservar memoria");
		}	
	}

/* Pick the people for every week */
//	printf("Ready to pick the double turns\n");
	aux_days_row = 0;
	for(w = 0; w < weeks; w++) {	/*FOR EVERY WEEK we fill the gaps*/

	     hole = -1;
	     aux_holes =  (days)*(times - 1) - min(  people_week[week_order[w]],(days)*(times - 1) );
	    for ( filled = 0; filled < aux_holes; filled ++) {
//		printf("Configuration %i, week %i, hole %i/%i \n", conf, week_order[w], filled + 1,(days)*(times - 1) - people_week[week_order[w]]);	
	/* First we try this greedy solution where we pick random people for the double turns */ 
		aux_sol = total_solutions[week_order[w]][top_order[w]];

		aleat = ((rand ()*(float)total_solutions[week_order[w]][top_order[w]].posibles[filled][0]/RAND_MAX));
		aux_i =  total_solutions[week_order[w]][top_order[w]].posibles[filled][aleat + 1];

		hole = 	findint (aux_sol.chose + hole +1,((times-1)*days) - hole, -1) + hole + 1;
		if ( pref_week[week_order[w]][hole] == aux_i) {  /* To see if the double turn is a preference */
			final_solutions[conf].preferences += 1;
		}

/*	for (i = 0; i < total_solutions[week_order[w]][top_order[w]].posibles[filled][0]; i++) {
			printf("%i ",total_solutions[week_order[w]][top_order[w]].posibles[filled][1+i]);
		}

		printf(" --> %i",aux_i);
*/

//		printf("\n");

/* 	HERE WE HAVE THE CONSTRAINT THAT A PERSON CANNOT WORK TWO DAYS IN A ROW */
/*...................................................................................*/
		for (i = max(hole - hole%(times- 1) - (times-1),0); i <= min(hole + 1 -hole%(times- 1) + (times-1),days*(times-1)); i++){
			if (aux_sol.chose[i] == aux_i ) {
				if (aux_days_row >= days_row ) {

					feas_conf = 0;
				}
				else {
					aux_days_row++;
				}
			}
		}
/*...................................................................................*/
		final_solutions[conf].more_turn[w][filled] = aux_i;
		times_worked[aux_i] +=1;
	    }
	}

/*	for (i = 0; i < total_people; i++){
		printf("%s --> %i \n", total_people_s[i], times_worked[i]);
	}
*/
	copy_vector_int(final_solutions[conf].week_order, week_order, weeks);
	copy_vector_int(final_solutions[conf].top_order, top_order, weeks);
	final_solutions[conf].variance = get_variance (times_worked, total_people);
/* IMPROVEMENTS:
	- WORK ON THE CHOSEN DOUBLE TURNS SO THAT:
		1- The double turns of a person are as sparse as posible
		2- Noone does triple - turn in a week
	- In funtion of these we can stablish if a conf is feasible or not.
	- DESIGN A WAY TO CHECK IF WE ALREADY HAD A CONFIGURATION SO THAT WE DONT SAVE SAME CONFIGURATIONS
*/

		
//	printf("Total preferences %i.  Variance %f \n", final_solutions[conf].preferences,final_solutions[conf].variance );

/* Now we check feasibility for no triple turn */
	for (w = 0; w < weeks; w++) {
		aux_holes =  (days)*(times - 1) - min(  people_week[week_order[w]],(days)*(times - 1) );
		for (j = 0; j < total_people; j++){
			if ( count_int(final_solutions[conf].more_turn[w], aux_holes, j) > 1) {
				feas_conf = 0;
			}
		}
	}
/* Now we check that noone works double turn 2 weeks in a row,
We are gonna let this happen a given number of times n_double */
	aux_double = 0;
	for (w = 0; w < weeks-1; w++) {
		reverse_order (order_aux, week_order, weeks);
		aux_holes =  (days)*(times - 1) - min(  people_week[w],(days)*(times - 1) );
		aux_holes2 =  (days)*(times - 1) - min(  people_week[w+1],(days)*(times - 1) );
		for (j = 0; j < aux_holes; j++){
			if ( count_int(final_solutions[conf].more_turn[order_aux[w+1]], aux_holes2, final_solutions[conf].more_turn[order_aux[w]][j]) >= 1){ 
				if (aux_double >= n_double) {
					feas_conf = 0;
				}
				else {
					aux_double++;
//					printf(" %i \n",aux_double);
				}
			}
		}
	}
		
	if (feas_conf == 1) {
		if (check_solution2 (final_solutions, conf, days, times, weeks) == 0) {
			conf += 1;
			printf("Solution %i FOUND\n", conf);
		}
		else{
			printf("Solucion REPETIDA \n");
		}
	}
	else {
		free(final_solutions[conf].week_order );
		free(final_solutions[conf].top_order);

		for (i = 0; i < weeks; i++) {
			free(final_solutions[conf].more_turn[i] );
		}
		free(final_solutions[conf].more_turn );
	}
	
	current_try += 1;
}
/* We are gonna have to speed this a lot more:
	- No checking if a solution is duplicated coz happends very rare, do it afterwards
	- No allocating and freeing memory, use aux variables or search solutions in an order.
	- Implement this in a function so that when its not feasible, we do "return" and start over.
	- Maybe change the way this kind of solution is written will do it faster
	- Apply a selecting policy:
*/
	printf(" Combinations Found %i \n",conf);

if (conf != 0 ) {
/* PICK THE BEST COMBINATIONS */
   	variances = (float *) malloc (conf * sizeof(float));			/* To write all the variances of the solutions */
    	top_conf_order = (int *)malloc (conf * sizeof(int));
	for (i = 0; i < conf; i++) {
		variances[i] = final_solutions[i].variance;
	}
	ordenar_float2(variances, top_conf_order, conf);

/*-------------------------------> Write the soltions in a file <-----------------------------*/
 
printf("****************************************************************************\n");
printf("********************* FINAL COMBINATIONS ***********************************\n");
printf("****************************************************************************\n");
for (i = 0; i < min(5,conf); i++ ) {
	copy_vector_char (salida, argv[2], strlen(argv[2]));
	salida[strlen(argv[2])] = i + '0';
	salida[strlen(argv[2]) + 1] = '\0';
	out_sol (salida , final_solutions[top_conf_order[i]],weeks, days,times,  cad,  people_week,  total_solutions, pref_week,total_people);
}
}
  /* -------------------->  Free Memory <-----------------------------*/

	fclose(pf);

	for (i = 0; i < days; i++) {
		for (j = 0; j < times; j++) {
			free (table[i][j].posibles);
			free (table_aux[i][j].posibles);
		}	
		free (table[i]);
		free (table_aux[i]);
	}


	for (i = 0; i < days; i++) {
		free (days_s[i]);
	}
	free(days_s);

	for (i = 0; i < times; i++) {
		free (times_s[i]);	
	}
	free (times_s);

	for (i = 0; i < total_people; i++) {
		free(total_people_s[i]);
	}
	free(total_people_s);

	
	for ( i = 0; i < weeks; i++){
		for ( j = 0; j < top_week[i]; j++){
			free(total_solutions[i][j].chose);
		}
		free(total_solutions[i]);
	}
	free (total_solutions);


	free (people_week);
	free (pref_week);

	free (top_pos);
	free (order);
	free (top_week);

	free(times_worked); 
//	free(week_order);
	free(order_aux);
//	free(top_order );

 	free(order_hours);
	free(doubled);

	for (w = 0; w < conf; w++){
		free(final_solutions[w].week_order );
		free(final_solutions[w].top_order);

		for (i = 0; i < weeks; i++) {
			free(final_solutions[w].more_turn[i] );
		}
		free(final_solutions[w].more_turn );
	}
	free (final_solutions);
/* Contraints */
	printf("Memoria liberada con exito \n");
    return 1; 
}










