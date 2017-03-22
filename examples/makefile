CC = gcc

CFLAGS = -Wall -g 


LFLAGS = -lpthread


SRCS = hello-pthread.c ##Listar los archivos .c Ejem: read.c example.c

OBJS = $(SRCS:.c=.o)

MAIN = hello-pthread ##Poner nombre del archivo donde está el main


.PHONY: clean

all:    $(MAIN)
	@echo  Simple compiler named hello-pthread has been compiled

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS)  -o $(MAIN) $(OBJS) $(LFLAGS) 

.c.o:
	$(CC) $(CFLAGS)  -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)