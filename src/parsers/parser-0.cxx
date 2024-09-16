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


enum Indicator{newLine, doubleNewLine, asterisk, doubleAsterisk};

void parser0(std::string& input, Document& theDocument){
  auto& output=theDocument.contents;

  std::vector<std::pair<Indicator, size_t>> foundIndicators;

  std::unordered_map<Indicator, size_t> nextIndicators;

  std::vector<Indicator> indicatorsToCheck={
  newLine, doubleNewLine, asterisk, doubleAsterisk
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
        case asterisk:
          nextIndicators[indicator]=input.find('*', index);
          break;
        case doubleAsterisk:
          nextIndicators[indicator]=input.find("**", index);
          break;
        default:
          std::cerr << "how did we even get here?\n";
          break;
      }
    }
    indicatorsToCheck.clear();

    //this will get messy fast :/

    Indicator closest;
    
    if(nextIndicators[newLine]<nextIndicators[asterisk]){
      closest=newLine;
    }else{
      closest=asterisk;
    }

    //if the closest doesnt exist ==> no idicators exits ==> we are at the end
    if(nextIndicators[closest]==std::string::npos){
      break;
    }

    switch(closest){
      case newLine:{
        indicatorsToCheck.push_back(newLine);
        indicatorsToCheck.push_back(doubleNewLine);
    

        //prioritize double newLines
        if(nextIndicators[doubleNewLine]==nextIndicators[newLine]){
          foundIndicators.push_back({doubleNewLine, nextIndicators[doubleNewLine]});
          index=nextIndicators[doubleNewLine]+2; //double newline is 2 characters
        }else{
          foundIndicators.push_back({newLine, nextIndicators[newLine]});
          index=nextIndicators[newLine]+1; //newline is 1 character
        }
      }
      break;

      case asterisk:{
        indicatorsToCheck.push_back(asterisk);
        indicatorsToCheck.push_back(doubleAsterisk);
    
        //prioritize double asterisks
        if(nextIndicators[doubleAsterisk]==nextIndicators[asterisk]){
          foundIndicators.push_back({doubleAsterisk, nextIndicators[doubleAsterisk]});
          index=nextIndicators[doubleAsterisk]+2; //double asterisk is 2 characters
        }else{
          foundIndicators.push_back({asterisk, nextIndicators[asterisk]});
          index=nextIndicators[asterisk]+1; //asterisk is 1 character
        }
      }
      break;

    default:
      std::cerr << "how did we even get here??\n";
      break;
      
    }
  }

  // for(auto& [indicator, position] : foundIndicators){
  //       std::cout << indicator << " == " << position << "\n";
    
  // }
  // return;

  size_t prevIndex{0};
  TextBlock flags;
  output.push_back(flags);
  for(auto& [indicator, position] : foundIndicators){
      // std::cout << indicator << " == " << position << "\n" << std::flush;

    switch(indicator){
      //ignore and treat as a space
      case newLine:
        input[position]=' ';
        break;

      //start a new paragraph, is 2 chars
      case doubleNewLine:
        std::cout << "\\n == " << position << "\n";
        output.back().contents=input.substr(prevIndex, position-prevIndex);
        output.push_back(flags);
        output.back().lineBreakLevel=1;
        prevIndex=position+2;
        break;

      //make it bold, is 1 char
      case asterisk:
        std::cout << "* == " << position << "\n";
        output.back().contents=input.substr(prevIndex, position-prevIndex);
        flags.isBold=!flags.isBold;
        output.push_back(flags);
        prevIndex=position+1;
        break;

      //make it emphatic, is 2 chars
      case doubleAsterisk:
        std::cout << "** == " << position << "\n";
        output.back().contents=input.substr(prevIndex, position-prevIndex);
        flags.isEmphatic=!flags.isEmphatic;
        output.push_back(flags);
        prevIndex=position+2;
        break;
    }
  }
  //dont forget the last one, there is no indicator for it... unless there actually is an indicator
  if(prevIndex!=input.size()-1){
    output.back().contents=input.substr(prevIndex);
  }else{
    output.pop_back();
  }

}

