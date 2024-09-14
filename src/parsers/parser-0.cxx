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


void parser0(std::string& input, Document& theDocument){
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
  bool readyForSegmentChange{true};
  bool segmentChange{true};

  bool reachedTheEnd{false};

  //current char
  char current{'\0'};
  //very next char
  char next={'\0'};
  for(size_t start{0}, current{0}, end{0};; current++){
    


    //we reached the end
    if(end+1==input.size()){
      reachedTheEnd=true;
    }else{
      current=input[end];
      next=input[end+1];
    }



    if(reachedTheEnd){
      //apply font stuff
      output.push_back(TextBlock());
      output.back().contents=input.substr(start, end-start);
      break;
    }

    
    switch(current){
      case '\n':
        if(next=='\n'){
          readyForSegmentChange=true;
          newLine=true;
        }else{
          //single newline, ignore it, make it a space
          //maybe not best practice
          current=' ';
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

        if(readyForSegmentChange){
          readyForSegmentChange=false;
          segmentChange=true;
        }
        end++;

        break;
    }

    if(segmentChange){

      
      //set the contents of the
      output.back().contents=input.substr(start, end-start);


      
      output.push_back(TextBlock());

      if(newLine){
        output.back().lineBreakLevel=1;
      }

      start=end;
    }else{
      output.back().contents=input.substr(start, end-start);
    
    
    }


    

  }


  












  

}
