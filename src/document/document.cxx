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

/*

bold:
(?<=(?<!\*)\*)(\s*\b)([^\*]+)(\b\s*)(?=\*(?!\*))

emphatic: start with one and only one "*", do not contain 2 or more "*", end with one and only 1 "*"
(?<=\*\*)([^\*][^\*]+)(?=\*\*)




*/


#include "document.hxx"
#include <algorithm>
#include <iostream>
#include <regex>



Document::Document(const std::string& name){
  setTitle(name);
}


Document::Document(const std::string& name, const std::string& text){
  setTitle(name);
  setContents(text);
}

Document::Document(const std::string& name, std::istream& text){
  setTitle(name);
  setContents(text);
}

Document::Document(const std::string& name, std::istream& text, const format& style){
  setTitle(name);
  setContents(text);
  setFormat(style);
}

Document::~Document(){
  //nothing to do here
  //TODO perhaps log the deletions of the documents somewhere
}

void Document::rename(const std::string& name){
  setTitle(name);
}

void Document::setName(const std::string& name){
  setTitle(name);
}

void Document::setTitle(const std::string& name){
  title=name;
}

void Document::setContents(const std::string& stuff){
  contents=stuff;
}

void Document::setContents(std::istream& contentStream){
  std::getline(contentStream, contents, '\0');
}

void Document::setFormat(const format& style){
  contentFormat=style;
}


void Document::resetLinks(const std::set<Document*>& allDocuments){

  auto oldForwardLinks=forwardLinks;
  resetForwardLinks(allDocuments);

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

Document::format Document::getFormat(){
  return contentFormat;
}

void Document::addBackwardLink(Document* document){
  backwardLinks.insert(document);
}

void Document::removeBackwardLink(Document* document){
  backwardLinks.erase(document);
}


void Document::resetForwardLinks(const std::set<Document*>& allDocuments){

  forwardLinks.clear();
  auto& output=forwardLinks;
  
  std::regex pattern("\\{([^\\}]*)\\{([^\\}]+)\\}([^\\}]*)\\}");
  std::smatch match;

  std::sregex_iterator regIterator(contents.begin(), contents.end(), pattern);
  std::sregex_iterator endIterator;


  for(;regIterator!=endIterator; regIterator++){

      //0 is the whole thing
      //1 is the tags
      //2 is the title (thats what we want)
      //3 is the displayed name
    auto documentTitle=regIterator->str(2);
    auto documentPointer{allDocuments.begin()};
    
    for(; documentPointer!=allDocuments.end(); documentPointer++){
      if((*documentPointer)->getTitle()==regIterator->str(2)){
        break;
      }
    }
    
    
    if(documentPointer!=allDocuments.end()){
      output.insert(*documentPointer);
    }
 
  }
}

std::set<Document*> Document::getMentionedDocuments(const std::set<Document*>& allDocuments){


  std::set<Document*> output;
  
  std::regex pattern("\\{([^\\}]*)\\{([^\\}]+)\\}([^\\}]*)\\}");
  std::smatch match;


  // (?<=\{)([^\{\}]+)(?=\})
  // (?<=(?<!\{)\{)[^{}]*(?=\}(?!\}))
  // \{([^{\]]*(?:\{(?:[^{\]]*\})[^\{\}]*)*\})[^\{\}]*\}

  std::sregex_iterator regIterator(contents.begin(), contents.end(), pattern);
  std::sregex_iterator endIterator;


  for(;regIterator!=endIterator; regIterator++){

      //0 is the whole thing
      //1 is the tags
      //2 is the title (thats what we want)
      //3 is the displayed name
    auto documentTitle=regIterator->str(2);
    auto documentPointer{allDocuments.begin()};
    
    for(; documentPointer!=allDocuments.end(); documentPointer++){
      if((*documentPointer)->getTitle()==regIterator->str(2)){
        break;
      }
    }
    
    
    if(documentPointer!=allDocuments.end()){
      output.insert(*documentPointer);
    }
 
  }

  return output;
}

