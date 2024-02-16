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

#include "document.hxx"
#include "textBlock.hxx"

void Document::processContents(std::istream& stream, const std::unordered_map<std::filesystem::path, Document*>& allDocuments){
  setContents(stream);
  processContents(allDocuments);
}


void Document::processContents(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  textBlocks.clear();
  
  TextBlock::fontFlags flags{TextBlock::Regular};

  int sequentialOpenCurlyBrackets{0};
  int sequentialClosedCurlyBrackets{0};

  int consecutiveNewLinesCount{0};
  int consecutiveAsteriskCount{0};

  bool startOfTextBlock{true};
  
    
  std::string currentTextBlock{""};
  
  for(const char& currentChar: contents){

    switch(0){
      case 0:
      
      if(currentChar=='\n'){
        consecutiveNewLinesCount++;
      }else{
      }

      if(currentChar=='*'){
        consecutiveAsteriskCount++;
      }else{
        //add a separator block
        if(consecutiveAsteriskCount>0){

          //if we are changing the format, make a new text block
          if(!startOfTextBlock){
            textBlocks.push_back(new TextBlock());
          }

          //set the type of separator          
          if(consecutiveAsteriskCount==1){
            static_cast<TextBlock*>(textBlocks.back())->fontFormat |= TextBlock::Bold;
          }else if(consecutiveAsteriskCount>=2){ // ** = *** = **** = ****...

            //NOTE EMPHATIC TEXT HERE vVVVVVVVVv
            static_cast<TextBlock*>(textBlocks.back())->fontFormat |= TextBlock::Bold;
            static_cast<TextBlock*>(textBlocks.back())->fontFormat |= TextBlock::Italic;
          }

          consecutiveAsteriskCount=0;
        }
      }      

      if(currentChar=='*'){
      
      default:


        //add a separator block
        if(consecutiveNewLinesCount>1){

          //if we are changing the format, make a new text block
          if(!startOfTextBlock){
            textBlocks.push_back(new SeparatorBlock());
          }
        
          //set the type of separator          
          if(consecutiveNewLinesCount==2){
            static_cast<SeparatorBlock*>(textBlocks.back())->separator=SeparatorBlock::NewLine;
          }else if(consecutiveNewLinesCount==3){
            static_cast<SeparatorBlock*>(textBlocks.back())->separator=SeparatorBlock::NewParagraph;
          }else if(consecutiveNewLinesCount>=4){
            static_cast<SeparatorBlock*>(textBlocks.back())->separator=SeparatorBlock::NewChapter;
          }
        
          consecutiveNewLinesCount=0;
          //dont care if we have ** or __ or whatever, newlines take presidence
          break;
        }


        startOfTextBlock=false;
        currentTextBlock+=currentChar;
        break;
    }
      
  }
  
}
