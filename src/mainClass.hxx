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

class MainClass{

  private:
    //variables:
    std::set<Document*> documents;
    std::string workingPath;
    
  public:

    //constructor, like a atSTartUp function
    MainClass();

    //deconstror
    ~MainClass();

    //the function that does all the logic, basically the main function
    void startProgram();


  private:
  
    //turns a string to a suitable name for a file
    std::string makeSuitableForFileName(std::string);
  
    // bool loadFile(std::string);
    //loads a document from a file path relative to the working path
    bool loadDocument(std::string);
  
    //makes a new document in the directory whose path is the working path
    bool makeDocument(std::string);
  
  
};
