SYS = Linux

CC = g++
DEBUG = -g
OWD = ~/projects/openworld
ODS = ~/projects/opendims
CFLAGS = -Wall -c $(DEBUG) -I $(OWD) -I $(ODS) -O3
LFLAGS = -Wall $(DEBUG) -O3

OBJS_MAIN = $(ODS)/dims/Dims.o $(ODS)/dims/Dimensions.o $(ODS)/dims/Dimensionless.o $(ODS)/dims/GlobalDimensions.o $(ODS)/measure/Unit.o $(ODS)/measure/Units.o $(OWD)/memory/Transients.o $(OWD)/sysdyn/TemporalVariable.o $(OWD)/sysdyn/Stock.o $(OWD)/multi/Distribution.o

all: conflict.o $(OBJS_MAIN)
	$(CC) $(LFLAGS) conflict.o $(OBJS_MAIN) -o conflict

c2: conflict2.o $(OBJS_MAIN)
	$(CC) $(LFLAGS) -lgsl -lgslcblas conflict2.o $(OBJS_MAIN) -o conflict2

#Inference rule - states a general rule for compiling .o files
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	\rm $(OBJS_MAIN)
	\rm *.o conflict
