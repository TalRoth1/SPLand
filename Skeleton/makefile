# Build all: clean, compile, and link
all: clean compile link

# Linking Rule
link:
	@echo "Linking source files"
	g++ -o bin/skeleton bin/main.o bin/Settlement.o bin/Simulation.o bin/Facility.o bin/Auxiliary.o bin/Plan.o bin/SelectionPolicy.o bin/Action.o

# Compile Rule
compile:
	@echo "Compile main"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/main.o src/main.cpp -Iinclude
	@echo "Compile Settlement"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Settlement.o src/Settlement.cpp -Iinclude
	@echo "Compile Simulation"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Simulation.o src/Simulation.cpp -Iinclude
	@echo "Compile Facility"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Facility.o src/Facility.cpp -Iinclude
	@echo "Compile Auxiliary"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Auxiliary.o src/Auxiliary.cpp -Iinclude
	@echo "Compile Plan"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Plan.o src/Plan.cpp -Iinclude
	@echo "Compile SelectionPolicy"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp -Iinclude
	@echo "Compile Action"
	g++ -g -Wall -Weffc++ -std=c++11 -c -o bin/Action.o src/Action.cpp -Iinclude

# Clean Rule
clean:
	@echo "Cleaning bin files"
	rm -rf bin/*
