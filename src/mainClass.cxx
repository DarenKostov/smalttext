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


#include "mainClass.hxx"
#include <iostream>
#include <string>


MainClass::MainClass(){
}
MainClass::~MainClass(){
}



void MainClass::startProgram(){

  for(int i=0; i<10; i++){
    auto document=new Document("Document "+std::to_string(i));
    document->setContents("This is the text inside Document "+std::to_string(i)
      +"\nThis Document link to {{Document "+std::to_string((i+1)%10)+"}}\n");
   
    documents.insert(document);
  
  }
  
  for(auto document : documents){
    document->resetLinks(documents);
    
    std::cout << document->getTitle() << ":\n";

    std::cout << "\tForward Links:\n";
    for(auto forward : document->getForwardLinks()){
      std::cout << "\t\t" << forward->getTitle() << "\n";
    }

    std::cout << "\tBackward Links:\n";
    for(auto forward : document->getBackwardLinks()){
      std::cout << "\t\t" << forward->getTitle() << "\n";
    }


    
  }

}


