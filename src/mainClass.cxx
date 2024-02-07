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
#include <cctype>
#include <iostream>
#include <regex>
#include <string>
#include <filesystem>
#include <fstream>
#include <tuple>


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

bool MainClass::loadDocument(std::string path){
  
  std::string identifier="";
  std::string title="";
  std::string tagLine="";
  std::string preSetting="";
  std::string postSetting="";
  std::string description="";
  std::string contents="";
  std::vector<std::string> tags;
  std::ifstream inputFileStream(path);

  if(inputFileStream.bad()){
    return false;
  }

  std::getline(inputFileStream, identifier);

  if(identifier!="<SMALTTEXT:0.0.0>"){
  // if(!(identifier=="<SMALTTEXT:0.0.0>" || identifier=="<SMALTTEXT>")){
    return false;
  }  
  
  
  inputFileStream.ignore(100, '=');
  inputFileStream.ignore(100, ' ');
  std::getline(inputFileStream, title, '\n');
  
  inputFileStream.ignore(100, '=');
  inputFileStream.ignore(100, ' ');
  std::getline(inputFileStream, tagLine, '\n');

  inputFileStream.ignore(100, '=');
  inputFileStream.ignore(100, ' ');
  std::getline(inputFileStream, preSetting, '\n');
  
  inputFileStream.ignore(100, '=');
  inputFileStream.ignore(100, ' ');
  std::getline(inputFileStream, postSetting, '\n');
  
  inputFileStream.ignore(100, '=');
  inputFileStream.ignore(100, ' ');
  std::getline(inputFileStream, description, '\n');
  
  std::getline(inputFileStream, contents, '\0');

  inputFileStream.close();

  
  auto document=new Document(title, contents);

  documents.insert(document);
  
  return true;
}


bool MainClass::makeDocument(std::string title){
  
  std::string supposedName;
  std::ifstream fileStream;

  //check which file path would be the best, check for duplicates, etc
  for(int i=0;; i++){
      
    std::string append="-"+std::to_string(i);
    if(i==0){
      append="";
    }
  
    supposedName=makeSuitableForFileName(title+append);
    fileStream.open(workingPath+"/"+supposedName);
    
    //does the file exist?
    if(fileStream.good()){

      std::string line="";
      std::getline(fileStream, line);
      std::getline(fileStream, line);
      fileStream.close();

      //does this document have the exact same title?
      if(title==line){
        //they do? there is nothing we can do, 2 documents cannot have the same title
        return false;
      }
        //They have diffrent titles, just the same file names?
        //proceed

    }

    if(i>=999){
      //if we reach this, we have unreasonably many # of documents with too similar (the same) titles
      return false;
    }
    
  }

  //create a new file
  std::ofstream newFileStream(workingPath+"/"+supposedName, std::ios::out);


  
  
  return true;
}


std::string MainClass::makeSuitableForFileName(std::string in){
  std::string out="";

  // // https://en.cppreference.com/w/cpp/algorithm/transform
  // transform(in.begin(), in.end(), out.begin(), tolower); 

  for(char character : in){
    character=tolower(character);
    
    if(!((character>=48 && character<=57) || (character>=97 && character<=122) || character==95 || character==45 || character==32))
      continue;
    
    if(character==32)
      character=45;
    
    out+=character;
  }
  
  out+=".txt";

  return out;
}











