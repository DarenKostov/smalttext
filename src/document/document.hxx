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
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "textBlock.hxx"

class Document{

  private:

    std::string title;    
    std::string contents;
  
    //what documents are linking to this document
    std::set<Document*> backwardLinks;

    //what this document is linking to
    std::set<Document*> forwardLinks;

    //the processed contents into separate text blocks
    std::vector<TextBlock*> textBlocks;

    //when mentioned in another document, how should the font appear?
    TextBlock preSetting;
       
    //the regex pattern for the presetting
    const static std::regex preSettingPattern;

  public:

    //==constructors & deconstructors
  
    //document constructor, requres a title
    Document(const std::string&);

    //document constructor, requres a title, and an istream with the contents.
    Document(const std::string&, std::istream&);

    //document deconstructor
    ~Document();
  
    //==Document info as a whole stuff


    //sets the preSetting
    void setPreSetting(const TextBlock&);
    void setPreSetting(const std::string&);

    //gets the preSetting
    const TextBlock& getPreSetting();
  
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
    const std::string& getContents();
  
    //gives you the processed textBlocks
    const std::vector<TextBlock*> getTextBlocks();
    
    //processes the contents inputed directly into text blocks and forward links
    void processContents(std::istream&, const std::unordered_map<std::filesystem::path, Document*>&);
  
    //re-processes the contents that were already stored into text blocks and forward links
    void reProcessContents(const std::unordered_map<std::filesystem::path, Document*>&);

    //process the contents in restricted mode
    void processContentsToTextBlocks(const std::unordered_map<std::filesystem::path, Document*>&);
  
    //processes a mention and returns a textblock stating the mentioned document
    //shifts the index to the last character of the mention
    TextBlock* processMention(size_t& currentIndex, const TextBlock*, const std::unordered_map<std::filesystem::path, Document*>& allDocuments);
  
    void processBrackets(int& currentIndex, std::vector<TextBlock*>&);
  
    //applies the macros to the contents
    void applyMacros();
 
    
};


