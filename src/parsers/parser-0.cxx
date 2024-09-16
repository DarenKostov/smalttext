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
#include <cstring>
#include <iostream>

//checks if the char is any of the given chars
bool isAnyOf(const char&, const char*);

void parser0(std::string& input, Document& theDocument){
  auto& output=theDocument.contents;


  // TextBlock current;
  // current.contents=input;
  // //TODO make sure you dont copy strings twice
  // output.push_back(current);

  // return;
 
  std::vector<std::pair<size_t, char>> segmentStops;
  std::string segmentIndicators{""};
  

  bool justGotAsterisk{false};
  bool justGotUnderscore{false};
  bool justGotTilde{false};
  bool justGotCaret{false};
  // bool justGot{false};
  // bool justGot{false};
  
  bool newLine{false};
  bool bold{false};
  bool underlined{false};

  bool italic{false};
  bool emphatic{false};

  bool crossed{false};

  bool readyForSegmentChange{true};
  bool segmentChange{true};

  bool reachedTheEnd{false};

  //current char
  char current{'\0'};
  //very next char
  char next={'\0'};
  for(size_t index{0};;index++){

    if(isAnyOf(input[index], "\n*~_^")){
    
    } 
    
    // index=input.find_first_of("\n*~_^", index);

    char& char1=input[index];
    char char2=input[index+1];

    //different different, we care only for the 1st one
    if(char1!=char2){
      switch(char1){
        case '\n':
          //make it a space
          break;
        case '*':
          //bold
          break;
        case '^':
          //superscript
          break;
        case '_':
          //italic
          break;
        case '~':
          //substript
          break;
      }
    
    //same values, we still care only for the 1st one :/
    }else{
      index++;
      switch(char1){
        case '\n':
          //new paragraph
          break;
        case '*':
          //emphasized
          break;
        case '^':
          //ignore
          break;
        case '_':
          //underline
          break;
        case '~':
          //crossed
          break;
      }
    
    }

    //rest of the string is clean
    if(index==std::string::npos){
      //WARNING EDGE CASE
      index=input.size()-1;
    }

    //we reached the end
    if(index==input.size()-1){
      //WARNING EDGE CASE
    
    }
}




  

}



bool isAnyOf(const char& single, const char* many){
  for(int i{0}; many[i]!='\0'; i++){
    if(many[i]==single){
      return true;  
    }
  }
  return false;
}
