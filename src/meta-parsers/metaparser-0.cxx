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
#include "../documents-and-texts/document.hxx"
#include "metaparsers.hxx"
#include <cmath>
#include <iostream>
#include <istream>
#include <unordered_map>


//give it hex base to dec base
int hexToDec(const char& in);

bool metaParser0(std::istream& stream, Document& newDocument){


  if(stream.bad()){
    return false;
    std::cerr << "bad stream\n";
  }

  
  std::string preSettingRaw;

  //meta is ahorrible name for the variables but that's what i thought of
  std::unordered_map<std::string, std::string*> metas={
    {"Title", &newDocument.title},
    {"PreSetting", &preSettingRaw},
    {"Description", &newDocument.description}
  };


  //there are no more than 3 metas (except contents)
  //if there are, there is something wrong
  for(int i{0}; i<3; i++){
    
    std::string currentMeta;
    std::getline(stream, currentMeta, '\n');

    //no "=", no document
    auto breakingOffPoint=currentMeta.find('=');
    if(breakingOffPoint==std::string::npos){
      std::cout << "wrong meta format\n";
      return false;
    }

    std::string currentMetaIdentifier=currentMeta.substr(0, breakingOffPoint);

    //no meta, no document
    //this also catches if contents is the name
    if(!metas.contains(currentMetaIdentifier)){
      return false;
      std::cout << "wrong meta name\n";
    }

    std::string currentMetaValue=currentMeta.substr(breakingOffPoint+2);
    //very readable
    metas[currentMetaIdentifier][0]=currentMetaValue;
  }


 
  std::string contentsLine;
  std::getline(stream, contentsLine, '\n');
  
  //no contents, no document
  if(!contentsLine.starts_with("Contents (leave next line empty)=")){
    std::cout << "no contents\n";
    return false;
  }


  //that should be an empty line but why check?
  std::getline(stream, contentsLine, '\n');

  //the presetting is just color for now
  try{
    newDocument.preSetting.color=std::stol(preSettingRaw, nullptr, 16);
  }catch(std::exception &err){
    //NOTE perhaps make the default value custom
    newDocument.preSetting.color=0x000000;
    // std::cerr << "preSetting color error:\n";
    // // std::cerr << preSettingRaw << "\n";
    // std::cerr << err.what() << "\n";
    // std::cerr << "end\n";
  }

  //everything is a success
  return true;

}

bool metaParser0Bypass(std::istream& stream){

  if(stream.bad()){
    return false;
    std::cerr << "bad stream\n";
  }

  for(int i{0}; i<5; i++){
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
  }

  return true;

}
