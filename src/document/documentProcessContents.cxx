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

/* TODO
   USE REGEX FOR THE MACROS!!!!!!
*/

#include "document.hxx"
#include "textBlock.hxx"

//makes the text font emphatic, or non emphatic depending on what it was previously
void toggleEmphatic(TextBlock::fontFlags&);

void Document::processContents(std::istream& stream, const std::unordered_map<std::filesystem::path, Document*>& allDocuments){
  setContents(stream);
  processContents(allDocuments);
}


void Document::processContents(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  textBlocks.clear();
  
  TextBlock::fontFlags flags{TextBlock::Regular};

  int sequentialOpenCurlyBrackets{0};
  int sequentialClosedCurlyBrackets{0};
  std::string documenttags{""};
  std::string documentTitle{""};
  std::string displayedTitle{""};

  
  int consecutiveNewLinesCount{0};
  int consecutiveAsteriskCount{0};
  int consecutiveUnderScoreCount{0};
    
  bool startOfTextBlock{true};
      
  std::string currentTextBlock{""};
  
  for(const char& currentChar: contents){

    switch(currentChar){

      case '\n':
        consecutiveNewLinesCount++;
        startOfTextBlock=true;
        break;

      
      case '*':
        consecutiveAsteriskCount++;
        startOfTextBlock=true;
        break;
 
      case '_':
        consecutiveUnderScoreCount++;
        startOfTextBlock=true;
        break;
      
      default:

        //if this is a brand new text block we are talking about
        if(startOfTextBlock){
          
          //===HANDLE NEWLINES FIRST
          //add a separator block
          if(consecutiveNewLinesCount>1){

            //newlines? new separator!
            textBlocks.push_back(new SeparatorBlock());
        
            //set the type of separator          
            if(consecutiveNewLinesCount==2){
              static_cast<SeparatorBlock*>(textBlocks.back())->separator=SeparatorBlock::NewLine;
            }else if(consecutiveNewLinesCount==3){
              static_cast<SeparatorBlock*>(textBlocks.back())->separator=SeparatorBlock::NewParagraph;
            }else if(consecutiveNewLinesCount>=4){
              static_cast<SeparatorBlock*>(textBlocks.back())->separator=SeparatorBlock::NewChapter;
            }
        
            consecutiveNewLinesCount=0;

            //we do care actually
            //dont care if we have ** or __ or whatever, newlines take presidence
            // break;
          }


          //add the new textblock, we got at least 1 character in it.
          textBlocks.push_back(new TextBlock());


          //===BOLD handling
          if(consecutiveAsteriskCount==1){
            //if not bold, bold; if bold, unbold
            flags ^= TextBlock::fontFlags::Bold;

          //===EMPHATIC handling
          }else if(consecutiveAsteriskCount>=2){
            //if not emphatic, emphatic; if emphatic, un-emphatic
            toggleEmphatic(flags);
          }
        
          //===ITALIC handling
          if(consecutiveAsteriskCount==1){
            //if not italic, italicize; if italic, un-italicize
            flags ^= TextBlock::fontFlags::Italic;

          //===UNDERLINE handling
          }else if(consecutiveAsteriskCount>=2){
            //if not underlined, underline; if underlined, un-underline
            flags ^= TextBlock::fontFlags::Underlined;
          }
                    
          static_cast<TextBlock*>(textBlocks.back())->fontFormat=flags;
        
          //reset these
          consecutiveAsteriskCount=0;
          consecutiveUnderScoreCount=0;
        
        }      
        
        startOfTextBlock=false;
        currentTextBlock+=currentChar;
        break;
    }
      
  }
  
}

void toggleEmphatic(TextBlock::fontFlags& flags){
  bool emphatic{false};

  //check if it's emphatic
  if(flags &= TextBlock::fontFlags::Bold)
    if(flags &= TextBlock::fontFlags::Italic)
      emphatic=true;

  //make emphatic
  flags |= TextBlock::Bold;
  flags |= TextBlock::Italic;

  //if it was previously emphatic, make it non emphatic
  if(emphatic){
  flags ^= TextBlock::Bold;
  flags ^= TextBlock::Italic;
  }

}

