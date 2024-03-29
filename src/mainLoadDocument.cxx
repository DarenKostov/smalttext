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

#include "document/document.hxx"
#include "document/textBlock.hxx"
#include "mainClass.hxx"


//TODO Make better tag system
void setDocumentTags(std::string&, std::vector<std::string>&);

bool MainClass::loadDocument_0_0_0(std::istream& inputStream, Document*& document){

  
  std::string title="";
  std::string tagLine="";
  std::string preSetting="";
  std::string description="";
  std::vector<std::string> tags;

  if(inputStream.bad()){
    return false;
  }
  
  
  std::string currentSetting{""};
  std::string currentSettingIdentifier{""};
  std::string currentSettingValue{""};
  while(true){
    
    std::getline(inputStream, currentSetting, '\n');

    //the actual contents of the documents are startting no more configs
    if(currentSetting.starts_with("Contents:")){
      break;
    }
    
    //if the space isnt found, this config line is invalid, skip it
    auto breakingOffPoint=currentSetting.find(' ');
    if(breakingOffPoint==std::string::npos){
      continue;
    }
    
    currentSettingIdentifier=currentSetting.substr(0, breakingOffPoint);
    currentSettingValue=currentSetting.substr(breakingOffPoint+1);

    if(currentSettingIdentifier=="Title="){
      title=currentSettingValue;
    
    }else if(currentSettingIdentifier=="Tags="){
      setDocumentTags(currentSettingValue, tags);
    
    }else if(currentSettingIdentifier=="PreSetting="){
      preSetting=currentSettingValue;
    
    }else if(currentSettingIdentifier=="Description="){
      description=currentSettingValue;
    
    }else{
      //TODO handle invalid config lines
    }
  
  
  }
  
  
  //the rest of the input stream is the contents of the document 
  document=new Document(title, inputStream);
  document->setPreSetting(preSetting);
  
  return true;
}


void setDocumentTags(std::string& input, std::vector<std::string>& list){

  auto breakingOffPoint=input.find(',');

  //while the current tag exists
  while(breakingOffPoint!=std::string::npos){
    
    //add current tag
    list.push_back(input.substr(0, breakingOffPoint+1));

    //remove the current tag from the input
    input=input.substr(breakingOffPoint+2);
  
    //get the current tag from the input
    breakingOffPoint=input.find(',');
    
  }

}
