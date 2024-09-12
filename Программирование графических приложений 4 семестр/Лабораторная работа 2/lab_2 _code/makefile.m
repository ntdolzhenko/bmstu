EXEC=trancatedCube
LIBS= -lX11
INCLUDES=trancatedCube.h
HEADDIR= -I.
CC=cc $(HEADDIR)
OBJECTS=disp.o geom.o main.o
all: $(OBJECTS) $(EXEC)
$(OBJECTS): $(INCLUDES)
$(EXEC): $(OBJECTS) $(INCLUDES)
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS)