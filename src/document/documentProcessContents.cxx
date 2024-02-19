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
#include <cctype>
#include <cmath>

//makes the text font emphatic, or non emphatic depending on what it was previously
void toggleEmphatic(TextBlock::fontFlags&);

//give it hex char, itll give you dec int; 0 if invalid
int hexToDec(const char&);

void Document::processContents(std::istream& stream, const std::unordered_map<std::filesystem::path, Document*>& allDocuments){
  setContents(stream);
  processContents(allDocuments);
}


void Document::processContents(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  textBlocks.clear();
  
  TextBlock::fontFlags flags{TextBlock::Regular};
  uint32_t textColor{0x000000ff};

  //the saved font color and flags "[save]"
  TextBlock::fontFlags savedFlags{TextBlock::Regular};
  uint32_t savedTextColor{0x000000ff};

  //"[clear]" should reset the flags and color
  
  //should the next text block have the saved color and flags? "[load]"
  bool loadTextState{false};
  
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
  
  const auto contentsLength=contents.size();
  for(int i{0}; i<contentsLength; i++){


    
    switch(contents[i]){

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
      
      
      //===SQUARE BRACKETS Handling
      case '[':
        i++;
        //make sure we are not at the end of the contents
        if(i>=contentsLength)
          break;
      
        switch(contents[i]){
          //===COLOR Handling
          case '#':
            i++;
            //make sure that the amount of charcters (6) is correct, otherwise, invalid color, no need to parse
            if(i+6>=contentsLength || contents[i+6]!=']')
              break;

            //first character is "d", most likely "defcol" (default color), no need to waste resources checking if it really is "defcol"
            if(contents[i]=='d'){
              textColor=DEFAULT_TEXTBLOCK_COLOR;
            }else{
            
              //reset the color
              textColor=0x000000ff;
          
              //add values to the color
              for(int j=0; j<6; j++){
                textColor+=hexToDec(contents[i+j])*std::pow(16, j+2);
              }
          
            }

            i+=6;
            startOfTextBlock=true;
            break;

          //===SAVE STATE Handling
          case 's':
            i+=4;
            //make sure that the amount of charcters (4) is correct, otherwise, invalid, no need to parse
            if(i>=contentsLength || contents[i]!=']')
              break;

            //We arent going to parse regardless, im just gonna assume that "save" is written, to save resources
            savedTextColor=textColor;
            savedFlags=flags;
            break;

          //===LOAD STATE Handling
          case 'l':
            i+=4;
            //make sure that the amount of charcters (4) is correct, otherwise, invalid, no need to parse
            if(i>=contentsLength || contents[i]!=']')
              break;

            //We arent going to parse regardless, im just gonna assume that "load" is written, to save resources
            startOfTextBlock=true;
            loadTextState=true;
            break;
        
          //===CLEAR STATE Handling
          case 'c':
            i+=4;
            //make sure that the amount of charcters (4) is correct, otherwise, invalid, no need to parse
            if(i>=contentsLength || contents[i]!=']')
              break;

            //We arent going to parse regardless, im just gonna assume that "clear" is written, to save resources
            startOfTextBlock=true;
            flags=TextBlock::fontFlags::Regular;
            textColor=DEFAULT_TEXTBLOCK_COLOR;
            break;

          //===URL Handling
          case 'u':
            //TODO
            break;

          //===FILE PATH Handling
          case 'f':
            //TODO
            break;
        
        }

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
        currentTextBlock+=contents[i];
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


/*This performed worse
int hexToDec(const char& in){
  if(in>='0' && in<='9'){
    return in-'0';
  }
  if(in>='a' && in<='f'){
    return in-'a'+10;
  }
  if(in>='A' && in<='F'){
    return in-'A'+10;
  }
  return 0;
}
*/

//this performed better
int hexToDec(const char& in){
  switch(in){
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    
    case 'a':
      return 10;
    case 'b':
      return 11;
    case 'c':
      return 12;
    case 'd':
      return 13;
    case 'e':
      return 14;
    case 'f':
      return 15;
  
    case 'A':
      return 10;
    case 'B':
      return 11;
    case 'C':
      return 12;
    case 'D':
      return 13;
    case 'E':
      return 14;
    case 'F':
      return 15;

    defualt:
      return 0;
  }

  return 0;
}

