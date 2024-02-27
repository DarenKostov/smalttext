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

#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "textBlock.hxx"

class Document{

  private:

    std::string title;    
    std::string contents;
  
    //what this document is linking to
    std::set<Document*> backwardLinks;

    //what document are linking to this file
    std::set<Document*> forwardLinks;

    //the processed contents into separate text blocks
    std::vector<TextBlock*> textBlocks;

    //is it a restricted or unrestricted document?
    //NOTE unrestricted allows for macros, while the restricted doesnt
    TextBlock::type typeOfDocument;

    //when mentioned in another document, how should the font appear?
    UnrestructedTextBlock preSetting;
       
  public:

    //==constructors & deconstructors
  
    //document constructor, requres a title.
    Document(const std::string&);

    //document constructor, requres a title and an istream with the contents.
    Document(const std::string&, std::istream&);

    //document deconstructor
    ~Document();
  
    //==Title stuff
  
    //sets the title of the document
    void rename(const std::string&);
    void setName(const std::string&);
    void setTitle(const std::string&);
  
    //gives you the title of the document
    std::string getTitle();
  

    //==Links stuff

    //resets the links from this document (what this document links to); provide it with all of the existing documents
    void resetLinks(const std::unordered_map<std::filesystem::path, Document*>&);

    //(re)sets the forward links (aka mentioned documents) given the contents of the document
    void resetForwardLinks(const std::unordered_map<std::filesystem::path, Document*>&);
  
    //gives you the documents linking to the document
    std::set<Document*>  getBackwardLinks();
    
    //gives you what documents this document is linking to
    std::set<Document*>  getForwardLinks();

    //adds a backward link to this document (if it doesnt exist)
    void addBackwardLink(Document*);

    //removes a backward link to this document (if it exists)
    void removeBackwardLink(Document*);
  

    //==Content stuff

    //sets the contents of the document
    void setContents(std::istream&);

    //gives you the contents of the document
    std::string getContents();
  
    //gives you the processed textBlocks
    const std::vector<TextBlock*> getTextBlocks();
    
    //processes the contents inputed directly into text blocks and forward links
    void processContents(std::istream&, const std::unordered_map<std::filesystem::path, Document*>&);
  
    //re-processes the contents that were already stored into text blocks and forward links
    void processContents(const std::unordered_map<std::filesystem::path, Document*>&);

    //applies the macros to the contents
    void applyMacros();
  
  





  
    
};

