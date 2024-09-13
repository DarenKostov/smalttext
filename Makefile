# https://www.cs.colostate.edu/~cs157/LectureMakefile.pdf
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90050

CXX= g++
# CXXFLAGS= -std=c++20 -lstdc++fs
CXXFLAGS= -std=c++20 -lstdc++fs -g
SFMLFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
OBJFILES= bin/main.o bin/licence.o bin/mainClass.o bin/mainDraw.o bin/mainUpdate.o bin/mainActions.o
PARSERFILES= bin/metaparser-0.o bin/parser-0.o
TARGET= bin/TEMPLATE


all: $(TARGET)


$(TARGET): $(OBJFILES) $(PARSERFILES)
	@echo FINAL BINARY:
	$(CXX) $(CXXFLAGS) $(SFMLFLAGS) $(OBJFILES) $(PARSERFILES) -o $(TARGET)


bin/main.o: src/main.cxx src/mainClass.hxx
	@echo MAIN:
	$(CXX) $(CXXFLAGS) src/main.cxx -c -o bin/main.o

bin/licence.o: src/licence.cxx
	@echo LICENCE:
	$(CXX) $(CXXFLAGS) src/licence.cxx -c -o bin/licence.o
	
	
bin/mainClass.o: src/mainClass.cxx src/mainClass.hxx
	@echo MAIN CLASS:
	$(CXX) $(CXXFLAGS) src/mainClass.cxx -c -o bin/mainClass.o

bin/mainDraw.o: src/mainDraw.cxx src/mainClass.hxx
	@echo MAIN DRAW:
	$(CXX) $(CXXFLAGS) src/mainDraw.cxx -c -o bin/mainDraw.o
	
bin/mainUpdate.o: src/mainUpdate.cxx src/mainClass.hxx
	@echo MAIN UPDATE:
	$(CXX) $(CXXFLAGS) src/mainUpdate.cxx -c -o bin/mainUpdate.o

bin/mainActions.o: src/mainActions.cxx src/mainClass.hxx
	@echo MAIN ACTIONS:
	$(CXX) $(CXXFLAGS) src/mainActions.cxx -c -o bin/mainActions.o
	

bin/metaparser-0.o: src/meta-parsers/metaparser-0.cxx
	@echo META PARSER 0:
	$(CXX) $(CXXFLAGS) src/meta-parsers/metaparser-0.cxx -c -o bin/metaparser-0.o
	
bin/parser-0.o: src/parsers/parser-0.cxx
	@echo PARSER 0:
	$(CXX) $(CXXFLAGS) src/parsers/parser-0.cxx -c -o bin/parser-0.o


clean:
	rm -f bin/*



.PHONY: clean all


