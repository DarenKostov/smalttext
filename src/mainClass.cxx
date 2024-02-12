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


MainClass::MainClass(const std::string& path){

  workingPath=path;

  identifierPattern.assign("<SMALTTEXT:([0-9]+\\.[0-9]+\\.[0-9]+)>");
  defaultFileExtention=".txt";

}
MainClass::~MainClass(){

  //make sure we are not making mery leaks  
  for(const auto& [path, document] : documents){
    delete document;
  }

  
}



void MainClass::startProgram(){

  loadProject();  

  printLinks();





}

void MainClass::loadProject(){

  for (auto filePath : std::filesystem::directory_iterator(workingPath)) {
    loadDocument(filePath);
  }

  for (auto& [path, document] : documents) {
    document->resetLinks(documents);
  }
}
 
bool MainClass::loadDocument(const std::filesystem::path& path){

  std::string identifier="";
  Document* document=nullptr;
  std::smatch match;
  std::string version;
  std::ifstream inputFileStream(path);

  if(inputFileStream.bad()){
    return false;
  }

  std::getline(inputFileStream, identifier);

  if(!std::regex_match(identifier, match, identifierPattern)){
    //identifier does not match, wrong file, return false
    return false;
  }
  version=match.str(1);

  if(version=="0.0.0"){
    loadDocument_0_0_0(inputFileStream, document);
  }else{
    //unknown version
    return false;
  }

  //if the document is already loaded, delete it to not make memory leaks
  if(documents.find(path)!=documents.end()){
    delete documents[path];
  }
  
  documents[path]=document;
  inputFileStream.close();

  return true;
}

bool MainClass::makeDocument(const std::string& title){
  
  std::string supposedName{title};
  std::filesystem::path supposedPath{workingPath};
  std::ifstream fileStream;
  
  //set up initial
  makeSuitableForAFileName(supposedName);
  supposedPath/=(supposedName+defaultFileExtention);
  
  
  //check which file path would be the best, check for duplicates, etc
  for(int attempt=0;; attempt++){
      
    //change the name a bit if there is already a file with the same name
    if(attempt!=0){
      supposedPath.replace_filename(supposedName+"-"+std::to_string(attempt)+defaultFileExtention);
    }
  
    fileStream.open(supposedPath);
    
    //does the file exist? (actually: did we sucessfully open the file?)
    // if(fileStream.good()){
    // if(fileStream.is_open()){
    if(fileStream){

      fileStream.close();
      fileStream.clear();

      //does this document have the exact same title?
      if(title==documents[supposedPath]->getTitle()){
        //they do? there is nothing we can do, 2 documents cannot have the same title
        return false;
      }
        //They have diffrent titles, just the same file names?
        //proceed

    }else{
      //close the file regardless if it was opened successfully
      fileStream.close();
      fileStream.clear();
    }

    if(attempt>=999){
      //if we reach this, we have unreasonably many # of documents with too similar (the same) titles
      return false;
    }
    
  }

  //create a new file
  std::ofstream newFileStream(supposedPath, std::ios::out);

  //fill in a template
  newFileStream << "<SMALTTEXT:0.0.0>\n";
  newFileStream << "Title= " << title << "\n";
  newFileStream << "Tags= \n";
  newFileStream << "PreSetting= \n";
  newFileStream << "PostSetting= \n";
  newFileStream << "Format= SmaltText\n";
  newFileStream << "Description= \n";
  newFileStream << "Contents:\n\n";

  newFileStream.close();
  
  return true;
}


void MainClass::makeSuitableForAFileName(std::string& in){
  std::string out="";

  // // https://en.cppreference.com/w/cpp/algorithm/transform
  transform(in.begin(), in.end(), out.begin(), tolower); 

  for(char character : in){
    character=tolower(character);
    
    if(!((character>='0' && character<='9') || (character>='a' && character<='z') || character=='_' || character=='-' || character==' '))
      continue;
    
    if(character==' ')
      character='-';
    
    out+=character;
  }
  
  in=out;

  // out+=".txt";
  // return out;
}


void MainClass::printLinks(){
  
  for(const auto&  [path, document] : documents){
    std::cout << document->getTitle() << ":\n";

    std::cout << "\tMentions:\n";
    for(auto forward : document->getForwardLinks()){
      std::cout << "\t\t" << forward->getTitle() << "\n";
    }

    std::cout << "\tMentioned by:\n";
    for(auto forward : document->getBackwardLinks()){
      std::cout << "\t\t" << forward->getTitle() << "\n";
    }
  }
}

void MainClass::printMentions(){
  printLinks();
}










