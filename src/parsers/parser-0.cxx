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


  // TextBlock current;
  // current.contents=input;
  // //TODO make sure you dont copy strings twice
  // output.push_back(current);

  // return;
 
  std::vector<std::pair<size_t, char>> segmentStops;
  std::string segmentIndicators{""};
  

  bool newLine{false};
  bool bold{false};
  bool italic{false};
  bool crossed{false};
  bool underlined{false};
  bool segmentChange{true};


  for(size_t previousIndex{0};;){
    
    auto currentIndex=input.find_first_of("\n*_~[^\0", previousIndex);
    if(currentIndex==std::string::npos){
      std::cerr << "how did we even get here?\n";
      return;
    }

    //current char
    const char& current{input[currentIndex]};
    //we reached the end
    if(input[previousIndex]=='\0'){
      break;
    }

    //very next char
    const char& next={input[currentIndex+1]};
    //next is safe becase it is at least \0


    switch(current){
      case '\n':
        if(next=='\n'){
          segmentChange=true;
          newLine=true;
        }
        break;
      case '*':
        if(next=='*'){
        
        }
        break;
      case '~':
        if(next=='~'){
        
        }
        break;
      case '_':
        if(next=='_'){
        
        }
        break;
      case '^':
        if(next=='^'){
        
        }
        break;
      case '[':
        break;
      case '{':
        break;

      default:
        std::cerr << "how did we even get here??\n";
        return;
        break;
    }

    if(segmentChange){
      output.push_back(TextBlock());
      if(newLine){
        output.back().lineBreakLevel=1;
      }
    
    
    }else{
      // if(newLine){
        output.back().contents+=input.substr(previousIndex, currentIndex-previousIndex);
        newLine=false;      
      // }
    
    }


    

    segmentStops.push_back({currentIndex, input[currentIndex]});
    previousIndex=currentIndex;
  }


  












  

}
