
# https://www.cs.colostate.edu/~cs157/LectureMakefile.pdf
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90050

CXX= g++
CXXFLAGS= -std=c++20 -lstdc++fs
SFMLFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
MAINOBJFILES= bin/main.o bin/licence.o bin/mainClass.o bin/mainDraw.o bin/mainUpdate.o bin/mainActions.o
OBJFILES= bin/mainLoadDocument.o bin/document.o bin/documentProcessContents.o bin/mainProcessTextBlocks.o
TARGET= bin/smalttext


all: $(TARGET)


install: all
	cp $(TARGET) /usr/bin/

$(TARGET): bin $(MAINOBJFILES) $(OBJFILES)
	@echo FINAL BINARY:
	$(CXX) $(CXXFLAGS) $(SFMLFLAGS) $(MAINOBJFILES) $(OBJFILES) -o $(TARGET)
	
bin:
	mkdir bin

bin/main.o: src/main.cxx
	@echo MAIN CPP:
	$(CXX) $(CXXFLAGS) src/main.cxx -c -o bin/main.o

bin/licence.o: src/licence.cxx
	@echo LICENCE:
	$(CXX) $(CXXFLAGS) src/licence.cxx -c -o bin/licence.o
	
bin/mainClass.o: src/mainClass.cxx src/mainClass.hxx src/mainLoadDocument.cxx src/document/document.hxx
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

bin/mainLoadDocument.o: src/mainLoadDocument.cxx src/mainClass.hxx src/document/document.hxx
	@echo LOAD DOCUMENTS:
	$(CXX) $(CXXFLAGS) src/mainLoadDocument.cxx -c -o bin/mainLoadDocument.o

bin/mainProcessTextBlocks.o: src/mainProcessTextBlocks.cxx src/mainClass.hxx src/document/document.hxx
	@echo TEXT BLOCKS PROCESSOR:
	$(CXX) $(CXXFLAGS) src/mainProcessTextBlocks.cxx -c -o bin/mainProcessTextBlocks.o

bin/document.o: src/document/document.cxx src/document/document.cxx
	@echo DOCUMENT CLASS:
	$(CXX) $(CXXFLAGS) src/document/document.cxx -c -o bin/document.o

bin/documentProcessContents.o: src/document/documentProcessContents.cxx src/document/documentProcessContents.cxx
	@echo DOCUMENT PROCESSOR CLASS:
	$(CXX) $(CXXFLAGS) src/document/documentProcessContents.cxx -c -o bin/documentProcessContents.o



clean:
	rm -f bin/*



.PHONY: clean install all


