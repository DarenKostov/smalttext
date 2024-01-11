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


#include "document.hxx"



Document::Document(std::string name){
  title=name;
}


Document::Document(std::string name, std::string text){
  title=name;
  contents=text;
}


Document::~Document(){
  //nothing to do here
  //TODO perhaps log the deletions of the documents somewhere
}

void Document::rename(std::string name){
  title=name;
}

void Document::setName(std::string name){
  rename(name);
}


void Document::setContents(std::string stuff){
  contents=stuff;
}

void Document::resetLinks(){
  //TODO reset links
}

std::string Document::getTitle(){
  return title;
}

std::string Document::getContents(){
  return contents;
}

std::set<Document*> Document::getBackwardLinks(){
  return backwardLinks;
}

std::set<Document*>  Document::getForwardLinks(){
  return forwardLinks;
}




