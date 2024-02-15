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
    std::vector<DocumentBlock*> textBlocks;
  
     
  public:

    //==constructors & deconstructors
  
    //document constructor, requres a title.
    Document(const std::string&);
  
    //document constructor, requres a title and contents.
    Document(const std::string&, const std::string&);

    //document constructor, requres a title and an istream with the contents.
    Document(const std::string&, std::istream&);

    //document deconstructor
    ~Document();
  
    //==setters
  
    //sets the title of the document
    void rename(const std::string&);
    void setName(const std::string&);
    void setTitle(const std::string&);
  
    //sets the contents of the document
    void setContents(const std::string&);
    void setContents(std::istream&);

    //resets the links from this document (what this document links to); provide it with all of the existing documents
    void resetLinks(const std::unordered_map<std::filesystem::path, Document*>&);

    //==getters

    //gives you the title of the document
    std::string getTitle();
  
    //gives you the contents of the document
    std::string getContents();
  
    //gives you the documents linking to the document
    std::set<Document*>  getBackwardLinks();
    
    //gives you what documents this document is linking to
    std::set<Document*>  getForwardLinks();

    //gives you the processed textBlocks
    const std::vector<TextBlock*> getTextBlocks();
    
    //==obtainers? adders? removers? misc?
  
    //(re)sets the forward links (aka mentioned documents) given the contents of the document
    void resetForwardLinks(const std::unordered_map<std::filesystem::path, Document*>&);

    //processes the contents into text blocks
    void processContents();

    //processes the contents inputed directly into text blocks
    void setAndProcessContents(std::istream&);
    void processContents(std::istream&);

    //processes the contents inputed directly into text blocks and forward links
    void refreshContentsAndLinks(std::istream&, const std::unordered_map<std::filesystem::path, Document*>&);
    void processContents(std::istream&, const std::unordered_map<std::filesystem::path, Document*>&);
  
    //adds a backward link to this document (if it doesnt exist)
    void addBackwardLink(Document*);
  
    //removes a backward link to this document (if it exists)
    void removeBackwardLink(Document*);



  
    
};

