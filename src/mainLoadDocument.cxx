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
#include "mainClass.hxx"


bool MainClass::loadDocument_0_0_0(std::istream& inputStream, Document*& document){

  
  std::string title="";
  std::string tagLine="";
  std::string preSetting="";
  std::string postSetting="";
  std::string description="";
  std::vector<std::string> tags;

  if(inputStream.bad()){
    return false;
  }
  
  
  inputStream.ignore(100, '=');
  inputStream.ignore(100, ' ');
  std::getline(inputStream, title, '\n');
  
  inputStream.ignore(100, '=');
  inputStream.ignore(100, ' ');
  std::getline(inputStream, tagLine, '\n');

  inputStream.ignore(100, '=');
  inputStream.ignore(100, ' ');
  std::getline(inputStream, preSetting, '\n');
  
  inputStream.ignore(100, '=');
  inputStream.ignore(100, ' ');
  std::getline(inputStream, postSetting, '\n');
  
  inputStream.ignore(100, '=');
  inputStream.ignore(100, ' ');
  std::getline(inputStream, description, '\n');
  
  
  //the rest of the input stream is the contents of the document 
  document=new Document(title, inputStream);

  
  return true;
}


