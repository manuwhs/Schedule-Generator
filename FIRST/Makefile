all: timetable

timetable.o: timetable.c
	gcc -c timetable.c -Wall

funciones.o: funciones.c
	gcc -c funciones.c -Wall

timetable: funciones.o timetable.o funciones.h 
	gcc -o timetable timetable.o funciones.o  -Wall -lm

