# Makefile for lab11

CMP = gcc
MAIN = project
FUNC = arcadefunc
EXEC = project
GRAPHIC = gfx

$(EXEC): $(FUNC).o $(MAIN).o $(GRAPHIC).o
	$(CMP) $(FUNC).o $(MAIN).o $(GRAPHIC).o -lX11 -lm -o $(EXEC)

$(FUNC).o: $(FUNC).c $(FUNC).h 
	$(CMP) -c $(FUNC).c -o $(FUNC).o 

$(MAIN).o: $(MAIN).c $(FUNC).h 
	$(CMP) -c $(MAIN).c -o $(MAIN).o 

clean:
	rm arcadefunc.o
	rm project.o
	rm $(EXEC)

