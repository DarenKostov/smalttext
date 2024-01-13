
# https://www.cs.colostate.edu/~cs157/LectureMakefile.pdf
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90050

CXX= g++
CXXFLAGS= -std=c++17 -lstdc++fs
OBJFILES= bin/main.o bin/licence.o bin/mainClass.o bin/document.o
TARGET= bin/smalttext


all: $(TARGET)


install: all
	cp $(TARGET) /usr/bin/

$(TARGET): bin $(OBJFILES)
	@echo FINAL BINARY:
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $(TARGET)
	
bin:
	mkdir bin

bin/main.o: src/main.cxx
	@echo MAIN CPP:
	$(CXX) $(CXXFLAGS) src/main.cxx -c -o bin/main.o

bin/licence.o: src/licence.cxx
	@echo LICENCE:
	$(CXX) $(CXXFLAGS) src/licence.cxx -c -o bin/licence.o
	
bin/mainClass.o: src/mainClass.cxx src/mainClass.hxx src/document/document.hxx
	@echo MAIN CLASS:
	$(CXX) $(CXXFLAGS) src/mainClass.cxx -c -o bin/mainClass.o

bin/document.o: src/document/document.cxx src/document/document.cxx
	@echo DOCUMENT CLASS:
	$(CXX) $(CXXFLAGS) src/document/document.cxx -c -o bin/document.o



clean:
	rm -f bin/*



.PHONY: clean install all


