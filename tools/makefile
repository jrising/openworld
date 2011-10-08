SYS = Linux

CC = mpic++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

OBJS_TIFF2TSV = convert.o ../datastr/Matrix.h ../datastr/FileFormats.h ../geotiff/commonLib.o ../geotiff/tiffIO.o ../memory/Transients.o

convert: $(OBJS_TIFF2TSV)
	$(CC) $(LFLAGS) $(OBJS_TIFF2TSV) -o convert

#Inference rule - states a general rule for compiling .o files
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	\rm *.o
