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


#include <set>
#include <string>


class Document{

  private:
    std::string title;    
    std::string contents;
  
    //what this document is linking to
    std::set<Document*> backwardLinks;

    //what document are linking to this file
    std::set<Document*> forwardLinks;

  public:

    //==constructors & deconstructors
  
    //document constructor, requres a title.
    Document(std::string);
  
    //document constructor, requres a title and contents.
    Document(std::string, std::string);

    //document deconstructor
    ~Document();
  
    //==setters
  
    //sets the title of the document
    void rename(std::string);
    void setName(std::string);
  
    //sets the contents of the document
    void setContents(std::string);

    //resets the links from this document (what this document links to); provide it with all of the existing documents
    void resetLinks(std::set<Document*>);


    //==getters

    //gives you the title of the document
    std::string getTitle();
  
    //gives you the contents of the document
    std::string getContents();
  
    //gives you the documents linking to the document
    std::set<Document*>  getBackwardLinks();
    
    //gives you what documents this document is linking to
    std::set<Document*>  getForwardLinks();


    //==obtainers? adders? removers? misc?
  
    //gives you all of the mentioned documents; provide it with all of the existing documents
    std::set<Document*> getMentionedDocuments(std::set<Document*>);

    //adds a backward link to this document (if it doesnt exist)
    void addBackwardLink(Document*);
  
    //removes a backward link to this document (if it exists)
    void removeBackwardLink(Document*);



  
    
};

