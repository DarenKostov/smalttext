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
#include <iostream>


void parser0(const std::string& input, Document& theDocument){
  auto& output=theDocument.contents;


  TextBlock current;

  int consecutiveNewlines{0};

  bool finishedWithCurrentTextBlock{true};
  
  
  size_t segmentStart{0};
  size_t segmentEnd{0};
  
  for(;;){
    
    auto potentialSegmentEnd=input.find_first_of("\n*_~[\0", segmentStart);
    if(potentialSegmentEnd==std::string::npos){
      std::cerr << "how did we even get here?\n";
      return;
    }
    
    switch(input[potentialSegmentEnd]){
      case '\n':
        consecutiveNewlines++;
        if(consecutiveNewlines>1){
          finishedWithCurrentTextBlock=true;
        }
        
      break;        

      default:
      //do absolutly nothing for now
      break;
    }
    

    if(finishedWithCurrentTextBlock){
      finishedWithCurrentTextBlock=false;
      output.back().contents=input.substr(startSegment, index-startSegment);
      output.push_back(TextBlock());
    }
  }

  current.contents=input;

  //TODO make sure you dont copy strings twice
  output.push_back(current);


}
