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
#include "meta-parsers/metaparsers.hxx"
#include "parsers/parsers.hxx"
#include "exporters/exporters.hxx"
#include <charconv>
#include <iostream>
#include <queue>
#include <regex>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>


MainClass::MainClass(const std::filesystem::path& projectPath){
  // window.create(sf::VideoMode(123, 123), "WXYZ");
  // mainView=sf::View(sf::FloatRect(0, 0, 123, 123));
  // mainView.setViewport(sf::FloatRect(0, 0, 1, 1));
  // window.setView(mainView);
  // window.setVerticalSyncEnabled(true);


  //loadding fonts
    
  if (!(
    font[Mono][Regular].loadFromFile("/usr/share/fonts/liberation/LiberationMono-Regular.ttf") &&
    font[Mono][Bold].loadFromFile("/usr/share/fonts/liberation/LiberationMono-Bold.ttf") &&
    font[Mono][Italic].loadFromFile("/usr/share/fonts/liberation/LiberationMono-Italic.ttf") &&
    font[Mono][Bold_Italic].loadFromFile("/usr/share/fonts/liberation/LiberationMono-BoldItalic.ttf")
    )) {
    std::cout << "error loading the mono font\n";
  }
  if (!(
    font[Serif][Regular].loadFromFile("/usr/share/fonts/liberation/LiberationSerif-Regular.ttf") &&
    font[Serif][Bold].loadFromFile("/usr/share/fonts/liberation/LiberationSerif-Bold.ttf") &&
    font[Serif][Italic].loadFromFile("/usr/share/fonts/liberation/LiberationSerif-Italic.ttf") &&
    font[Serif][Bold_Italic].loadFromFile("/usr/share/fonts/liberation/LiberationSerif-BoldItalic.ttf")
    )) {
    std::cout << "error loading the serif font\n";
  }
  if (!(
    font[Sans][Regular].loadFromFile("/usr/share/fonts/liberation/LiberationSans-Regular.ttf") &&
    font[Sans][Bold].loadFromFile("/usr/share/fonts/liberation/LiberationSans-Bold.ttf") &&
    font[Sans][Italic].loadFromFile("/usr/share/fonts/liberation/LiberationSans-Italic.ttf") &&
    font[Sans][Bold_Italic].loadFromFile("/usr/share/fonts/liberation/LiberationSans-BoldItalic.ttf")
    )) {
    std::cout << "error loading the sans font\n";
  }

  



  workingPath=projectPath;
  currentDocument=nullptr;
  
  loadFilesIntoLibrary(workingPath);

  
  // return;

  for(auto& [title, document]: documents){
    currentDocument=&document;
    std::cout << "title: " << document.title << "\n";
    break;
  }

}
MainClass::~MainClass(){
}



void MainClass::startProgram(){

  if(currentDocument==nullptr){
    return;
  }

  std::string output{""};

  convertToMarkdown(*currentDocument, output);
  
  std::cout << output << "\n";
  return;

  std::cout << "New document:\n";
  std::cout << "Title:" << currentDocument->title << "\n";
  std::cout << "Contents:\n";

  for(const auto& content : currentDocument->contents){
    std::cout << "\tContent:\n";
    std::cout << content.contents;
    std::cout << "\n\tEnd\n";
  }


  //"load all documents" magic here



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


