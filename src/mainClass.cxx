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
#include "documents-and-texts/document.hxx"
#include "meta-parser/metaparsers.hxx"
#include "meta-parsers/metaparsers.hxx"
#include "parsers/parsers.hxx"
#include <charconv>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>


MainClass::MainClass(const std::filesystem::path& projectPath){
  window.create(sf::VideoMode(123, 123), "WXYZ");
  mainView=sf::View(sf::FloatRect(0, 0, 123, 123));
  mainView.setViewport(sf::FloatRect(0, 0, 1, 1));
  window.setView(mainView);
  window.setVerticalSyncEnabled(true);

  workingPath=projectPath;

  
}
MainClass::~MainClass(){
}



void MainClass::startProgram(){


  //load all documents magic here


  loadFile(workingPath);

  for(auto& [title, document]: documents){
    std::cout << "New document:\n";
    std::cout << "Title:" << title << "\n";
    std::cout << "Contents:\nSOF";
    std::cout << document.contents[0].contents << "\n";
    std::cout << "EOF\n";
    
  }

  return;

  clock.restart();

  while(window.isOpen()){

    //drawing 66666-15fps 33333-30fps 16666-60fps 11111-90fps 8333-120fps 4166-240fps 
    usleep(16666);

    performActions();
    update();
    draw();


  }


}


//TODO make refresh contents
void MainClass::loadFile(const std::filesystem::path& path){

  std::string contentsRaw{""};

  //dont worry, performance hit of copying later should be minimal as we are not copying the contents
  Document newDocument;
  
  std::ifstream fileIn;
  fileIn.open(path);
  
  //TODO Parse Version
  int metaParserVersion{0};
  int parserVersion{0};
  
  switch(metaParserVersion){
    metaParser0(fileIn, newDocument);
  }
  
  //get the whole contents because strings are faster than streams in this case
  std::getline(fileIn, contentsRaw, '\0');
  fileIn.close();

  //TODO resolve collisions
  documents[newDocument.title]=newDocument;

  switch(parserVersion){
    parser0(contentsRaw, documents[newDocument.title]);
  }


}

