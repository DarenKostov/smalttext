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

#include <cstdint>
#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>


class Document{

  public: 

    enum format{SmaltText, MarkDown, Unknown};

    struct TextBlock{
  
      enum fontFlags{
        Regular=0        << 0, //no flags
        Bold=1           << 0,
        Italic=1         << 1,
        Underlined=1     << 2,
        StrickeThrough=1 << 3,
        SubScripts=1     << 4,
        SuperScript=1    << 5,
        CodeBlock=1      << 6,
        Url=1            << 7, //treat the contents as a url to a website
        FilePath=1       << 8, //treat the contencts as path to a local file
      };      
    
      fontFlags fontFormat{Regular};
      int fontSize{13};
      uint32_t color{0x000000ff};
      std::string content{""};
      Document* link{nullptr};
      
    };
  
  private:
    std::string title;    
    std::string contents;
  
    //what this document is linking to
    std::set<Document*> backwardLinks;

    //what document are linking to this file
    std::set<Document*> forwardLinks;

    //the processed contents into separate text blocks
    std::vector<TextBlock*> textBlocks;
  

  
  private:

    format contentFormat;
    
  public:

    //==constructors & deconstructors
  
    //document constructor, requres a title.
    Document(const std::string&);
  
    //document constructor, requres a title and contents.
    Document(const std::string&, const std::string&);

    //document constructor, requres a title and an istream with the contents.
    Document(const std::string&, std::istream&);

    //document constructor, requres a title, an istream with the contents, and the format of the contents.
    Document(const std::string&, std::istream&, const format&);

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

    //sets the format of the document
    void setFormat(const format&);

    //==getters

    //gives you the title of the document
    std::string getTitle();
  
    //gives you the contents of the document
    std::string getContents();
  
    //gives you the documents linking to the document
    std::set<Document*>  getBackwardLinks();
    
    //gives you what documents this document is linking to
    std::set<Document*>  getForwardLinks();

    //gets the format of the document
    format getFormat();

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
  
    //adds a backward link to this document (if it doesnt exist)
    void addBackwardLink(Document*);
  
    //removes a backward link to this document (if it exists)
    void removeBackwardLink(Document*);



  
    
};

