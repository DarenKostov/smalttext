/*
Copyright (c) 2024 Daren Kostov

This file is part of smalttext

smalttext is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

smalttext is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with smalttext
If not, see <https://www.gnu.org/licenses/>.
*/


#include "mainClass.hxx"

#include <iostream>
#include <csignal>
#include <filesystem>

//prints the licence
void printLicence();
//prints all flags
void printHelp();
//prints the version
void printVersion();


//at exit free the MainClass object 
void atExitFree(MainClass*);

static void freeVariable(){

  std::cout << "\e[93mAbout to Free Main\n\e[0m";
  atExitFree(NULL);
}

void atExitFree(MainClass* in){

  //store a static pointer to our Main
  static MainClass* eternalPtr;


  //if we give it a valid pointer, store it; if not, delete it
  if(in){
    std::cout << "\e[93mStoring Main\n\e[0m";
    eternalPtr=in;
    atexit(freeVariable);
  }else{
    std::cout << "\e[93mFreeing Main\n\e[0m";
    delete eternalPtr;
  }
}




void signal_handler(int signal_num){

  std::cout << "\n\e[93mSignal Handler!\n";
  std::cout << "Signal: " << signal_num << "\n\e[0m";
  exit(signal_num);
}


int main(int argc, char **argv){

  //if no folder is selected, select the current directory
  std::string projectPath{"./"};
  
  //check all flags
  for(int i=1; i<argc; i++){
    std::string thisArg=std::string(argv[i]);
    if(thisArg=="-l" || thisArg=="--licence"){
      printLicence();
      return 0;
    }else if(thisArg=="-h" || thisArg=="--help"){
      printHelp();
      return 0;
    }else if(thisArg=="-v" || thisArg=="--version"){
      printVersion();
      return 0;
    }else{
      projectPath=thisArg;
    }
  }


  std::cout << "\e[93mstart of program\n\e[0m";

  signal(SIGINT, signal_handler);

  if(projectPath[0]!='/'){
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90050
    projectPath=std::filesystem::current_path().generic_string()+'/'+projectPath;

  }


  MainClass* mainInstance=new MainClass(projectPath);
  atExitFree(mainInstance);

  mainInstance->startProgram();
  

}

//FUNCTION DEFINITION


void printHelp(){
  std::cout << "-h, --help: this help\n";
  std::cout << "-l, --licence: print the licence\n";
  std::cout << "-v, --version: prints the version\n";

}


void printVersion(){
  std::cout << "Copyright (c) Daren Kostov\n";
  std::cout << "GPLv3\n";
  std::cout << "SMALTTEXT\n";
    
  //Version Major.Minor.Patch
  std::cout << "Version 0.0.0\n";

}
