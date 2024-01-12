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
#include <algorithm>
#include <regex>



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

void Document::resetLinks(std::set<Document*> allDocuments){

  auto oldForwardLinks=forwardLinks;
  forwardLinks=getMentionedDocuments(allDocuments);

  std::set<Document*> removedForwardLinks;
  std::set<Document*> addedForwardLinks;

  //https://en.cppreference.com/w/cpp/algorithm/set_difference  
  std::set_difference(oldForwardLinks.begin(), oldForwardLinks.end(), forwardLinks.begin(), forwardLinks.end(), std::inserter(removedForwardLinks, removedForwardLinks.begin()));
  std::set_difference(forwardLinks.begin(), forwardLinks.end(), oldForwardLinks.begin(), oldForwardLinks.end(), std::inserter(addedForwardLinks, addedForwardLinks.begin()));
  
  //fix the backward links of the documents we were linking to forwardly
  for(auto document : addedForwardLinks){
    document->addBackwardLink(this);
  }
  for(auto document : removedForwardLinks){
    document->removeBackwardLink(this);
  }


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


void Document::addBackwardLink(Document* document){
  backwardLinks.insert(document);
}

void Document::removeBackwardLink(Document* document){
  backwardLinks.erase(document);
}



std::set<Document*> Document::getMentionedDocuments(std::set<Document*> allDocuments){

  std::set<Document*> output;
  
  std::regex pattern("\\{\\{([^\\}]+)\\}\\}");
  std::smatch match;

  std::sregex_iterator regIterator(contents.begin(), contents.end(), pattern);
  std::sregex_iterator endIterator;


  for(;regIterator!=endIterator; regIterator++){
    
    // https://en.cppreference.com/w/cpp/language/lambda
    auto sameTitle=[&](Document* in){
      return (in->getTitle())==(regIterator->str(1));
    };

    // https://en.cppreference.com/w/cpp/algorithm/find
    //if the title exsits as a title on any document, add it to the output
    auto result=std::find_if(allDocuments.begin(), allDocuments.end(), sameTitle);
    if(result!=allDocuments.end()){
      output.insert(*result);
    }
 
  }

  return output;
}

