# modify this makefile for your implementation
#   as described in the handout

llist.o:  list.h llist.c
	gcc -c llist.c

sq_slow.o: llist.o sq.h sq_slow.c
	gcc -c sq_slow.c

driver: driver.c sq_slow.o 
	gcc driver.c sq_slow.o llist.o -lc -o driver

llist_fast.o: list.h llist_sq.c
	gcc -c llist_sq.c

sq.o: llist_sq.o sq.h sq.c
	gcc -c sq.c

fdriver: driver.c sq.o 
	gcc driver.c sq.o llist_sq.o -lc -o driver_fast

clean:
	rm -f *.o driver
