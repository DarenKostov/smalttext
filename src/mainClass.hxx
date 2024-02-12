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

#include "document/document.hxx"
#include <filesystem>
#include <regex>
#include <unordered_map>

class MainClass{

  private:
    //variables:
    std::unordered_map<std::filesystem::path, Document*> documents;
    std::filesystem::path workingPath;


    //the pattern used to get the version of a document
    std::regex identifierPattern;

  public:

    //constructor, like a atSTartUp function
    MainClass(const std::string&);

    //deconstror
    ~MainClass();

    //the function that does all the logic, basically the main function
    void startProgram();


  private:

    //prints the relationship between documents
    void printLinks();
    void printMentions();
    
    //turns a string to a suitable name for a file
    std::string makeSuitableForAFileName(std::string);
  
    //makes a new document in the directory whose path is the working path
    bool makeDocument(std::string);

    //loads every file from the working directory
    void loadProject();
  
    //loads a document from a file path relative to the working path
    bool loadDocument(const std::filesystem::path&);
  
    //loads a document of version WXYZ
    bool loadDocument_0_0_0(std::istream&, Document*&);
  
  
};
