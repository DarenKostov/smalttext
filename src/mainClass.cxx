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
#include "document/document.hxx"
#include <iostream>
#include <regex>
#include <string>
#include <filesystem>
#include <fstream>


MainClass::MainClass(){

  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90050
  workingPath=std::filesystem::current_path().generic_string();
  workingPath+="/example";

}
MainClass::~MainClass(){

  //make sure we are not making mery leaks  
  for(auto document : documents){
    delete document;
  }

  
}



void MainClass::startProgram(){

  
  for (auto file : std::filesystem::directory_iterator(workingPath)) {
    loadDocument(file.path());
  }

  for (auto document : documents) {
    document->resetLinks(documents);
  }

  
  
  for(auto document : documents){
    std::cout << document->getTitle() << ":\n";

    std::cout << "\tForward Links:\n";
    for(auto forward : document->getForwardLinks()){
      std::cout << "\t\t" << forward->getTitle() << "\n";
    }

    std::cout << "\tBackward Links:\n";
    for(auto forward : document->getBackwardLinks()){
      std::cout << "\t\t" << forward->getTitle() << "\n";
    }
  }

}

bool MainClass::loadDocument(std::string relativePath){
  
  std::string path=relativePath;
  std::string title="";
  std::string contents="";
  std::string firstLine="";
  std::string tagLine="";
  std::vector<std::string> tags;
  std::ifstream inputFileStream(path);

  if(inputFileStream.bad()){
    return false;
  }

  std::getline(inputFileStream, firstLine);

  if(firstLine!="<SMALTTEXT>"){
    return false;
  }  
  
  std::getline(inputFileStream, tagLine);
  std::getline(inputFileStream, title);
  std::getline(inputFileStream, contents, '\0');
  inputFileStream.close();

  
  auto document=new Document(title, contents);

  documents.insert(document);
  
  return true;
}


bool MainClass::makeDocument(std::string name){




}













