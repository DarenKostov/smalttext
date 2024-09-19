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
#include <fstream>
#include <regex>
#include "meta-parsers/metaparsers.hxx"
#include "parsers/parsers.hxx"
#include "exporters/exporters.hxx"



std::tuple<bool, int, int, int> parseVersion(std::string);

bool MainClass::loadFileMeta(const std::filesystem::path& path){
  
  bool success{true};

  //dont worry, performance hit of copying should be minimal, this function does not process actual contents
  Document newDocument;
  
  std::ifstream fileIn;
  fileIn.open(path);
  
  //Parse Version
  int metaParserVersion{0};
  int parserVersion{0};
  int dontCare{0};
  std::string versionRaw{""};
  std::getline(fileIn, versionRaw, '\n');
  
  std::tie(success, metaParserVersion, parserVersion, dontCare)=parseVersion(versionRaw);
  newDocument.version={metaParserVersion, parserVersion, dontCare};
  

  //no success, no file
  if(!success){
    fileIn.close();
    if(metaParserVersion==0){
      std::cerr << "are you sure this is a smalttext file?\n";
    }else{
      std::cerr << "version format is wrong\n";
    }
    return false;
  }

  //should I use array with function pointers?
  switch(metaParserVersion){
    case 0:
      success=metaParser0(fileIn, newDocument);
      break;
    default:
      std::cerr << "unavaliable meta parser version\n";
      return false;
  }

  if(documents.find(newDocument.title)!=documents.end()){
    std::cerr << "duplicate title will not add \"" << newDocument.title << "\" again\n"; 
    return false;
  }

  newDocument.filePath=path;
  documents[newDocument.title]=newDocument;

  return false;
}

bool MainClass::loadFileContents(Document* document){

  bool success{true};
  std::string contentsRaw{""};
  std::ifstream fileIn;
  fileIn.open(document->filePath);

  int metaParserVersion{0};
  int parserVersion{0};
  int dontCare{0};
  std::tie(metaParserVersion, parserVersion, dontCare)=document->version;

  
  //dont care about version
  fileIn.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  

  //dont care about meta stuff either
  switch(metaParserVersion){
    case 0:
      success=metaParser0Bypass(fileIn);
      break;
    default:
      std::cerr << "unknown meta parser (bypass) version? how.\n";
      std::cerr << "version: " << metaParserVersion << "\n";
      return false;
    }

  
  if(!success){
    fileIn.close();
    std::cerr << "some sort of problem occured while bypassing the meta data stuff";
    return false;
  }


  //get the whole contents because strings are faster than streams in this case
  std::getline(fileIn, contentsRaw, '\0');
  fileIn.close();


  //clear the contents, we will be filling them in the next step
  document->contents.clear();

  switch(parserVersion){
    case 0:
      // *&  :/
      parser0(contentsRaw, *document);
      // std::cout << "externally ==" << document->contents.size();
      break;
    default:
      std::cerr << "unavaliable parser version: " << parserVersion << "\n";
      return false;
  }


  return true;

}

void MainClass::loadFilesIntoLibrary(const std::filesystem::path& initPath){

  //get the meta stuff
  for (auto& entry : std::filesystem::recursive_directory_iterator(initPath)){
    // std::cout << "[[" << entry.path() << "\n";
    if(entry.is_regular_file()){
      loadFileMeta(entry.path());
    }
  }

  //get the content stuff
  for(auto& [title, document] : documents){
    loadFileContents(&document);
  }

}


std::tuple<bool, int, int, int> parseVersion(std::string in){

  //maybe save this pattern as a constant?
  std::regex pattern(R"(<SMALTTEXT:([0-9]+)\.([0-9]+)\.([0-9]+)>)");
  std::smatch match;


  if(std::regex_search(in, match, pattern)){

    if(match.size()!=4){
      return {false, 1, 0, 0};
    }
    
    int metaParser{0};
    int parser{0};
    int iteration{0};

    //according to the regex these should be numbers, so maybe no need for try & catch?
    try{
      metaParser=std::stoi(match.str(1));
      parser=std::stoi(match.str(2));
      iteration=std::stoi(match.str(3));
    }catch(std::exception &err){
      return {false, 1, 0, 0};
    }

  return {true, metaParser, parser, iteration};

  }

  return {false, 0, 0, 0};

}


