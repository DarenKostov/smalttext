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
#include <unordered_map>



Document::Document(const std::string& name){
  setTitle(name);
}



Document::Document(const std::string& name, std::istream& text){
  setTitle(name);
  setContents(text);
}

Document::~Document(){

  for(const auto& textBlock : textBlocks){
    delete textBlock;
  }
  
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

void Document::setContents(std::istream& contentStream){
  std::getline(contentStream, contents, '\0');
}


void Document::resetLinks(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

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


void Document::addBackwardLink(Document* document){
  backwardLinks.insert(document);
}

void Document::removeBackwardLink(Document* document){
  backwardLinks.erase(document);
}


void Document::resetForwardLinks(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  forwardLinks.clear();
  
  std::regex pattern("\\{([^\\}]*)\\{([^\\}]+)\\}([^\\}]*)\\}");
  std::smatch match;

  std::sregex_iterator regIterator(contents.begin(), contents.end(), pattern);
  std::sregex_iterator endIterator;


  for(; regIterator!=endIterator; regIterator++){

      //0 is the whole thing
      //1 is the tags
      //2 is the title (thats what we want)
      //3 is the displayed name
    auto mentionedDocumentTitle=regIterator->str(2);
    Document* documentPointer{nullptr};
    
    for(const auto& [dontCare, document] : allDocuments){
      if(document->getTitle()==mentionedDocumentTitle){
        forwardLinks.insert(documentPointer);
        break;
      }
    }
    
  }
}


//TODO nested macros are not handled
//Make them work in the future!
void Document::applyMacros(){

  //might change the assignment pattern
  std::regex getMacroPattern("^(\\/\\/|#)\\$(.*):=(.*)$");

  std::unordered_map<std::string, std::string> macros;

  //iterators
  std::sregex_iterator macroIterator(contents.begin(), contents.end(), getMacroPattern);
  std::sregex_iterator endIterator;
  

  //collect all the definitions
  for(; macroIterator!=endIterator; macroIterator++){
    macros[macroIterator->str(2)]=macroIterator->str(3);
  }

  //apply all the macros
  for(const auto& [name, replaceWith] : macros){

    std::string pattern{"{"+name+"}"};
    std::size_t replaceLength{replaceWith.size()};

    //apply the macro wherever it appears
    for(std::string::size_type i{0}; i!=std::string::npos; i=contents.find(pattern, i)){

      //Handle escaping
      if(i!=0 && contents[i-1]=='\\'){
        i++;
        continue;
      }
      
      contents.replace(i, replaceLength, pattern);

      //lets not waste time trying to match the thing we just replaced
      i+=replaceLength;
    }
  }
  

  
}




