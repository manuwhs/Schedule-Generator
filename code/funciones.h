
/* For every time (morning, afternoon, not working) of every day, it tells me the people doing it */

typedef struct {
	char ** people;
	char ** days;
	char ** times;
	char ** total_people;
	} cadenas;

typedef struct {
	int * week_order;
	int * top_order;	/* To know which people top_solution we took from each week */
	int ** more_turn;	/* The number of the person to do second turn that we took for every week and hole
				in the order of top_orders and week_order and in every week by position of the -1 */
				/* double[weeks][posible at every turn of the week] */
	int preferences;	/* TOTAL number of preferences achived */
	float variance;		/* Variance of the number of ours worked by the people */
	}sol2;			/* This structure is used for choosing the best global solution */
				/*Once selected it is easily combined with the corresponding top_solutions to
				generate the final output */

typedef struct {
	int sorted;
	int people;		/* Possible people to work there */
	int *posibles;		/* List of posible people that can work there */
	} timing;
typedef struct {
	int person;		/* Person we are prunning everywhere */
	int day;		/* Day that person is working at */
	int time;		/* Time that person is working at*/
	} pruned;

typedef struct {
	pruned time[10];	/* We place here the people we have been pruning, if its a -1 it means "double turn" */
	int * chose;		/* This characterices the solution, chose[days * times] has in the position
				   [i*days + j*times] the person that ocupies day i and time j.
				   If it's a -1 it means its double turn time */
	int preferences;	/* Number of preferences succeded*/
	int ** posibles;	/* For every "hole" in the week, we create this array with the number
				of the people that can fill that hole.
				We can know the number of holes in a given week as (10 - people)
				possible[i][0] will have the number of people that can fill that hole */
				/* The first -1 correspond to the first posibles[0], and so on */
	} sol;

typedef struct {
	int d;		/* Days*/
	int t;		/* Times */
	int p;		/* People */
	int s;		/* Solved times so far */
	pruned prune;	/* Time to prune */
	
	timing ** table; /* Table with all the constraints */
	sol  * solu;	/* Current solution */
	} params;		


void zero_char (char * array, int length); 
void zero_int (int * array, int length);  

void get_items (char * data, int * capacity, int * items, int ** weights, int ** values); 
void get_items2 (char * archivo, int * capacity, int * items, int ** weights, int ** values);
void get_points (char * archivo, int * n_nodes, float ** points);
void load_solution (char * archivo, int n_nodes, int * solution, float * best);

void print_nodes (int * solution,int n_nodes ,float best, char * archivo);

void print_solution (char * solution,int items ,int best, char * archivo) ;

char * obtener_datos(char * archivo);  

int max(int a, int b);
float maxf (float * array, int num);  /* Give it the array and will give the best */
int min(int a, int b);

int dec2str(char  * cad, int digit);
int str2dec(char  * cad, char digit); 

int count_char(char * array, int len, int value);
int findint(int * array, int len, int value);
int count_int(int * array, int len, int value);

int ordenar_int (int * lista, int * orden, int len);
int ordenar_float (float * lista, int * orden, int len);
int ordenar_float2 (float * lista, int * orden, int len);
int ordenar_int2 (int * lista, int * orden, int len);

int reordenar_int (int * lista, int * orden, int len);
int reordenar_float (float * lista, int * orden, int len);
int reverse_order (int * order2, int * orden, int len);

int desreordenar_char (char * lista, int * orden, int len); 
int desreordenar_int (int * lista, int * orden, int len);

int copy_vector_char(char * duplicado, char * original, int num);
int copy_vector_int(int * duplicado, int * original, int num);
int copy_vector_float(float * duplicado, float * original, int num);

int cmp_int(int * array1, int * array2, int len) ;

int feas_prune (params * pa, int * num_infeas, int ini);
int prune (params * pa);
int take_choice (params * pa, int *n_preferences, pruned * preferences);
timing ** create_table (int days, int people, int times);
int init_table (int days, int people, int times, int ** constraints, timing ** table);
int print_out (sol solution, int people, int days, int times, cadenas cad, timing ** table );

int permutate_pref (pruned * preferences, int n_preferences);
int check_solution (sol * solutions, int n_solution, int days, int times);
int print_top_sol (sol solution, int w, int days, int times, cadenas cad, int * people_week,int ** pref_week);
int permutate_weeks (int weeks, int * top_week, int * week_order, int * top_order);

float get_variance (int * times_worked, int weeks);
int out_sol (char * fichero , sol2 gold, int weeks, int days, int times, cadenas cad, int * people_week, sol ** total_solutions, int ** pref_week, int total_people);
int check_solution2 (sol2 * solutions, int conf, int days, int times, int weeks);

