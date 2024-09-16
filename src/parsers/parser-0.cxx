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
#include <unordered_map>

//checks if the char is any of the given chars
bool isAnyOf(const char&, const char*);

std::pair<std::string, size_t> getClosest(std::unordered_map<std::string, size_t>);


enum Indicator{newLine, doubleNewLine};

void parser0(std::string& input, Document& theDocument){
  auto& output=theDocument.contents;

  std::vector<std::pair<Indicator, size_t>> foundIndicators;

  std::unordered_map<Indicator, size_t> nextIndicators={
    {newLine, 0},
    {doubleNewLine, 0},
  };

  std::vector<Indicator> indicatorsToCheck={
  newLine, doubleNewLine
  
  };
  
  //get all the initial values
  for(auto& [indicator, pos]: nextIndicators){
    pos=input.find(indicator);
  }


  for(size_t index{0};;){

    //update indicators that need to be updated
    for(auto& indicator : indicatorsToCheck){
      switch(indicator){
        case newLine:
          nextIndicators[indicator]=input.find('\n', index);
          break;
        case doubleNewLine:
          nextIndicators[indicator]=input.find("\n\n", index);
          break;
        default:
          std::cerr << "how did we even get here?\n";
          break;
      }
    }
    indicatorsToCheck.clear();

    //== ALL BELOW: if newlines are the closest
    indicatorsToCheck.push_back(newLine);
    indicatorsToCheck.push_back(doubleNewLine);
    
    //if the closest doesnt exist ==> no idicators exits ==> we are at the end
    if(nextIndicators[newLine]==std::string::npos){
      break;
    }

    //prioritize double newLines
    if(nextIndicators[doubleNewLine]==nextIndicators[newLine]){
      foundIndicators.push_back({doubleNewLine, nextIndicators[doubleNewLine]});
      index=nextIndicators[doubleNewLine]+2; //double newline is 2 characters
    }else{
      foundIndicators.push_back({newLine, nextIndicators[newLine]});
      index=nextIndicators[newLine]+1; //newline is 1 character
    }
    
  }

  size_t prevIndex{0};
  TextBlock flags;
  output.push_back(flags);
  for(auto& [indicator, position] : foundIndicators){

    switch(indicator){
      //ignore and treat as a space
      case newLine:
        input[position]=' ';
        break;

      //start a new paragraph, is 2 chars
      case doubleNewLine:
        std::cout << indicator << " == " << position << "\n";
        output.back().contents=input.substr(prevIndex, position-prevIndex);
        output.push_back(flags);
        output.back().lineBreakLevel=1;
        prevIndex=position+2;
        break;
    }
  }
  //dont forget the last one, there is no indicator for it
  output.back().contents=input.substr(prevIndex);

}

void parser0_old2(std::string& input, Document& theDocument){

  //flag is probably not the best name as flag would be the thing that this indicates
  //is unordered_map the best in this case performance wise? Perhaps a std::set of std::pair?
  std::unordered_map<std::string, size_t> nextFlag={
  {"\n", 0},
  {"\n\n", 0},
  // {"*", 0},
  // {"**", 0},
  // {"_", 0},
  // {"__", 0},
  // {"~", 0},
  // {"~~", 0},
  // {"^", 0},
  // {"", 0},
  // {"", 0},
  // {"", 0},
  // {"", 0},
  // {"", 0},
  };

  //get all the new(initial) values
  for(auto& [pattern, pos]: nextFlag){
    pos=input.find(pattern);
  }


  std::pair<std::string, size_t> currentFlag;

  
  bool reachedTheEnd{false};
  for(size_t index{0};;){

    //reset the current flag indicator
    currentFlag={"", std::string::npos};
    
    //currentFlag is previous flag at this point
    nextFlag[currentFlag.first]=input.find(currentFlag.first, index);

    currentFlag=getClosest(nextFlag);

    
    //if we dont have a flag indicator, we have reached the end
    if(currentFlag.second==std::string::npos){

      //add the last string
      break;
    }

    //perhaps enums would have been better :/
    if(currentFlag.first=="\n"){
      
    
    
    }else if(currentFlag.first=="\n\n"){

    }


  }

    
  
}



std::pair<std::string, size_t> getClosest(std::unordered_map<std::string, size_t> all){

  std::pair<std::string, size_t> closest;

  for(auto& [pattern, pos]: all){
    if(pos<=closest.second){
      //longer patterns get priority
      closest={pattern, pos};
    }
  }

  return closest;
}


void parser0_old(std::string& input, Document& theDocument){
  auto& output=theDocument.contents;


  // TextBlock current1;
  // current1.contents=input;
  // //TODO make sure you dont copy strings twice
  // output.push_back(current1);

  // return;
 

  int asterisksSoFar{false};
  int underscoresSoFar{false};
  int tildesSoFar{false};
  int caretsSoFar{false};
  int newLinesSoFar{false};
  
  //bold, italic, etc.-- lets call them flags
  bool gettingNewFlags{false};

  //if gettingNewFlags is false, disregard the value of this
  bool firstTimeGettingNewFlags{true};

  bool segmentChange{false};

  bool reachedTheEnd{false};

  TextBlock currentFlags;
  output.push_back(currentFlags);
  
  //current char
  char current{'\0'};
  for(size_t startIndex{0}, endIndex{0}, index{0};;index++){

    char& current{input[index]};


    //asume we are getting a new segment
    // std::cout << current<<"=" << std::flush;


    switch(current){
      case '\n':
        gettingNewFlags=true;
        newLinesSoFar++;
        break;
      case '*':
        gettingNewFlags=true;
        asterisksSoFar++;
        break;
      case '^':
        gettingNewFlags=true;
        caretsSoFar++;
        break;
      case '_':
        gettingNewFlags=true;
        underscoresSoFar++;
        break;
      case '~':
        gettingNewFlags=true;
        tildesSoFar++;
        break;

      case '\0':
        reachedTheEnd=true;
        break;

      default:

        //getting normal characters? no longer getting flags
        if(gettingNewFlags){
          segmentChange=true; 
          gettingNewFlags=false;
        }else{
          endIndex=index+1;
        }

      
    }

    //if we are getting new flags, we are already onto another segment
    if(segmentChange){

      segmentChange=false;

      //==CHECK ALL FLAGS AND SET THEM APPROPRIATLY

      int charsAmount=asterisksSoFar+caretsSoFar+underscoresSoFar+tildesSoFar+newLinesSoFar;

      if(charsAmount-newLinesSoFar==0){
        if(newLinesSoFar==1){
          //only 1 newline? ignore it
          input[index-1]=' ';
          currentFlags.lineBreakLevel=0;
          newLinesSoFar=0;

          //continue, no new segments
          continue;
      
        //more than 1? new paragraph segment (more than 1 is implied by segmentChange==true and no ather characters being found)
        }else{
          currentFlags.lineBreakLevel=1;
          newLinesSoFar=0;
          index++;
      
        }
      }

      output.back().contents=input.substr(startIndex, endIndex-startIndex);
      startIndex=index-1;
      output.push_back(currentFlags);

    }      

    if(reachedTheEnd){
      output.back().contents=input.substr(startIndex, endIndex-startIndex);
      return;
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
