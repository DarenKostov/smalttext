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

#include "parsers.hxx"


void parser0(const std::string& input, Document& theDocument){
  auto& output=theDocument.contents;


  TextBlock current;

  int consecutiveNewlines{0};

  bool finishedWithCurrentTextBlock{true};
  
  /*
    perhaps use find with starting position
    https://en.cppreference.com/w/cpp/string/basic_string/find
  */
  
  for(size_t startSegment{0}, index{0}; index<input.size(); index++){
    
    switch(input[index]){
      case '\n':
    
      break;        

      default:
      //do absolutly nothing for now
      if(finishedWithCurrentTextBlock){
        finishedWithCurrentTextBlock=false;
        output.back().contents=input.substr(startSegment, index-startSegment);
        output.push_back(TextBlock());
      }
    
    }
    
  }

  current.contents=input;

  //TODO make sure you dont copy strings twice
  output.push_back(current);


}
