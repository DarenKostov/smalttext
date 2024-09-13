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

#include <cstring>
#include <filesystem>
#include <iostream>
#include <csignal>

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

  std::filesystem::path projectPath{std::filesystem::current_path()};

  //check all flags
  for(int i=0; i<argc; i++){
    if(strcmp(argv[i], "-l")==0 ||
       strcmp(argv[i], "--licence")==0){
      printLicence();
      return 0;
    }else if(strcmp(argv[i], "-h")==0 ||
             strcmp(argv[i], "--help")==0){
      printHelp();
      return 0;
    }else if(strcmp(argv[i], "-v")==0 ||
             strcmp(argv[i], "--version")==0){
      printVersion();
      return 0;
    }else{

      // must be project directory
      if(argv[i][0]=='/'){
        projectPath=argv[i];
      }else{
        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90050
        projectPath=std::filesystem::current_path()/argv[i];
      }
    
    }

  }


  std::cout << "\e[93mstart of program\n\e[0m";

  signal(SIGINT, signal_handler);


  MainClass* mainInstance=new MainClass(projectPath);
  atExitFree(mainInstance);

  mainInstance->startProgram();
  

}

//FUNCTION DEFINITION


void printHelp(){


  std::cout << "smalttext [options] [folder] [options]\n";
  
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
