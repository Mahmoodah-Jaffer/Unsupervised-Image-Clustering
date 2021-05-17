CC = g++
CCFLAGS=-std=c++11

clusterer: driver.o clusterer.o #making an executable file that can be run
	$(CC) $(CCFLAGS) -o clusterer driver.o clusterer.o 

driver.o: driver.cpp clusterer.o
	$(CC) $(CCFLAGS) -c -o driver.o driver.cpp 

clusterer.o: clusterer.cpp clusterer.h
	$(CC) $(CCFLAGS) -c -o clusterer.o clusterer.cpp 

run: #change command line parameters here
	./clusterer Gradient_Numbers_PPMS -o testing -k 20 -bin 4
clean:
	@rm -f *.o #removing object files
	@rm clusterer #removing the executable produced 