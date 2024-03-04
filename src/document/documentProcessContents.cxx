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
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <unordered_map>

const std::regex Document::preSettingPattern{std::regex(R"(\*\*)|(__)|(\*)|(_)|(~~)|(~)|(\^)|(\[\#[0-9a-fA-F]{6}\])|(\[#[rgbymcoptlPLwBG]\])|(\[\#.+\])")};
/* example: 

  **__*_~~~^[#778899][#t][#pink]

*/

//makes the text font emphatic, or non emphatic depending on what it was previously
void toggleEmphatic(TextBlock::fontFlags&);

//TODO make ths hex char to hex uint32_t
//give it hex char, itll give you dec int; 0 if invalid
int hexToDec(const char&);

//give it a word, itll give you a color. the word does not have to be complete
uint32_t wordColorToHex(const char*);

//give it a char, itll give you a color.
uint32_t charColorToHex(const char&);

//counts the consecutive characters starting from an index, if they dont with a space returns 0
//be aware, the index inputed is being changed
int countConsecutiveCharactersBeforeSpace(const std::string& input, std::size_t& index, const std::size_t& size, const char& character);

//makes all chars lower
char* allTolower(char*);

ExtendedTextBlock* Document::processMention(size_t& i, const TextBlock* previousTextBlock, const std::unordered_map<std::filesystem::path, Document*>& allDocuments){
  ExtendedTextBlock* output{new ExtendedTextBlock()};

  Document* mentionedDocument{nullptr};
  
  std::string tagsOfMentionedDocument{""};
  std::string nameOfMentionedDocument{""};
  std::string displayedTitleOfMentionedDocument{""};

  //currenty we are at '{'

  for(i++; contents[i]!='{' && i<contents.size(); i++){
    tagsOfMentionedDocument+=contents[i];
  }
  //currenty we are at '{'

  for(i++; contents[i]!='}' && i<contents.size(); i++){
    nameOfMentionedDocument+=contents[i];
  }
  //currenty we are at '}'

  for(i++; contents[i]!='}' && i<contents.size(); i++){
    displayedTitleOfMentionedDocument+=contents[i];
  }

  //find the document with the given name
  for(const auto& [dontCare, document]: allDocuments){
    if(document->getTitle()==nameOfMentionedDocument){
      mentionedDocument=document;
      break;
    }
  }
  

  output->fontFormat |= previousTextBlock->fontFormat;
  output->fontFormat |= mentionedDocument->getPreSetting().fontFormat;
  output->color=mentionedDocument->getPreSetting().color;
  output->documentLink=mentionedDocument;

  if(displayedTitleOfMentionedDocument!=""){
    output->contents=displayedTitleOfMentionedDocument;
  }else{
    output->contents=nameOfMentionedDocument;
  }

  return output;
  
}

void Document::processContents(std::istream& stream, const std::unordered_map<std::filesystem::path, Document*>& allDocuments){
  setContents(stream);
  reProcessContents(allDocuments);
}


void Document::reProcessContents(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  //delete all previous textBlocks
  for(const auto& textBlock : textBlocks){
    delete textBlock;
  }
  textBlocks.clear();

  if(typeOfDocument==TextBlock::Lite){
    applyMacros();
    processContentsLite(allDocuments);
  }else{
    processContentsExtended(allDocuments);
  }

}

void Document::processContentsLite(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  TextBlock::fontFlags flags{TextBlock::Regular};

  int sequentialOpenCurlyBrackets{0};
  int sequentialClosedCurlyBrackets{0};
  std::string documenttags{""};
  std::string documentTitle{""};
  std::string displayedTitle{""};

  
  int consecutiveNewLinesCount{0};
  int consecutiveAsteriskCount{0};
  int consecutiveUnderScoreCount{0};
  int consecutiveCaretCount{0};
  int consecutiveTildeCount{0};
 
  bool startOfTextBlock{true};
      
  std::string currentTextBlockContents{""};

  
  const auto contentsLength=contents.size();
  for(std::size_t i{0}; i<contentsLength; i++){
    

    switch(contents[i]){

      case '\n':
        consecutiveNewLinesCount++;
        //only start a new word if this is the 2nd or 3rd or etc new line
        //ignorre single new lines
        if(consecutiveNewLinesCount>1){
          startOfTextBlock=true;
        }
        break;

      
      case '*':
        consecutiveAsteriskCount++;
        startOfTextBlock=true;
        break;
 
      case '_':
        consecutiveUnderScoreCount++;
        startOfTextBlock=true;
        break;
      
      case '~':
        consecutiveTildeCount++;
        startOfTextBlock=true;
        break;
      
      case '^':
        consecutiveCaretCount++;
        startOfTextBlock=true;
        break;

      case '{':
        //add the mention, without changing any of the CURRENT formatting
        textBlocks.push_back(processMention(i, textBlocks.back(), allDocuments));

        //the next chcrecter will start the next textblock with the CURRENT formatting
        startOfTextBlock=true;
        break;

      //===COMMENT Handler
      case '/':
        //are we at the start of a new line?
        if(consecutiveNewLinesCount>0){
          i++;
          //is there another "/" after this one?
          if(i<contentsLength && contents[i]=='/'){
            //skip the line
            consecutiveNewLinesCount--;
            for(; i<contentsLength && contents[i]!='\n'; i++);
          }else{
            i--;
          }
        }
        break;
          
      //===COMMENT Handler
      case '#':
        //are we at the start of a new line?
        if(consecutiveNewLinesCount>0){
          //skip the line
          consecutiveNewLinesCount--;
          for(; i<contentsLength && contents[i]!='\n'; i++);
        }
        break;
      
      //===ESCAPING Handler
      case '\\':
        i++;
        if(i>=contentsLength){
          break;
        }

        //this should be at least a TextBlock for the most part
        static_cast<TextBlock*>(textBlocks.back())->contents+=contents[i];
        break;
        
      default:

        //if this is a brand new text block we are talking about
        if(startOfTextBlock){
          
          //===HANDLE NEWLINES FIRST
          //add a separator block
          if(consecutiveNewLinesCount>1){
            
            //newlines? new separator!
            textBlocks.push_back(new TextBlock());
        
            //TODO I don't like this approach, should change it later

            //new paragraph
            if(consecutiveNewLinesCount==2){
              textBlocks.back()->contents="\n";
            //new section
            }else if(consecutiveNewLinesCount==3){
              textBlocks.back()->contents="\n\n";
            //new chapter
            }else if(consecutiveNewLinesCount>=4){
              textBlocks.back()->contents="\n\n\n";
            }
        
            //reset this
            consecutiveNewLinesCount=0;

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
                    
          //===SUB-SCRIPT handling
          if(consecutiveTildeCount==1){
            flags ^= TextBlock::fontFlags::SubScript;

          //===STRIKE-THROUGH handling
          }else if(consecutiveTildeCount>=2){
            flags ^= TextBlock::fontFlags::StrikeThrough;
          }

          //===SUPER-SCRIPT handling
          if(consecutiveCaretCount>=1){
            flags ^= TextBlock::fontFlags::SuperScript;
          }
        
          textBlocks.back()->fontFormat=flags;
          
        
          //reset these
          consecutiveAsteriskCount=0;
          consecutiveUnderScoreCount=0;
          consecutiveUnderScoreCount=0;
          consecutiveCaretCount=0;
          consecutiveTildeCount=0;
 
          //we are no longer at start of a new block, we just added it
          startOfTextBlock=false;
        }      
        
        //single newlines are ignorred and treated as spaces
        if(consecutiveNewLinesCount==1){
          consecutiveNewLinesCount=0;
          textBlocks.back()->contents+=' ';
        }else{
          //add the character to this textBlock
          textBlocks.back()->contents+=contents[i];
        }

        break;
    }
  }
}


void Document::processContentsExtended(const std::unordered_map<std::filesystem::path, Document*>& allDocuments){

  //do nothing for now
  
  /*
  
  TextBlock::fontFlags flags{TextBlock::Regular};
  uint32_t textColor{0x000000ff};

  //the saved font color and flags "[save]"
  TextBlock::fontFlags savedFlags{TextBlock::Regular};
  uint32_t savedTextColor{0x000000ff};

  //"[clear]" should reset the flags and color
  
  //should the next text block have the saved color and flags? "[load]"
  bool loadTextState{false};
  
  //the heading of current line
  int currentHeading{4};
  
  //the quote level, 0 means no quote
  int currentQuoteLevel{0};
  
  int sequentialOpenCurlyBrackets{0};
  int sequentialClosedCurlyBrackets{0};
  std::string documenttags{""};
  std::string documentTitle{""};
  std::string displayedTitle{""};

  
  int consecutiveNewLinesCount{0};
  int consecutiveAsteriskCount{0};
  int consecutiveUnderScoreCount{0};
  int consecutiveCaretCount{0};
  int consecutiveTildeCount{0};
 
  bool startOfTextBlock{true};
      
  std::string currentTextBlock{""};
  
  const auto contentsLength=contents.size();
  for(std::size_t i{0}; i<contentsLength; i++){


    
    switch(contents[i]){

      case '\n':
        consecutiveNewLinesCount++;
        //only start a new word if this is the 2nd or 3rd or etc new line
        //ignorre single new lines
        if(consecutiveNewLinesCount>1){
          startOfTextBlock=true;
        }
        break;

      
      case '*':
        consecutiveAsteriskCount++;
        startOfTextBlock=true;
        break;
 
      case '_':
        consecutiveUnderScoreCount++;
        startOfTextBlock=true;
        break;
      
      case '~':
        consecutiveTildeCount++;
        startOfTextBlock=true;
        break;
      
      case '^':
        consecutiveCaretCount++;
        startOfTextBlock=true;
        break;

      //===COMMENT Handler
      case '/':
        //are we at the start of a new line?
        if(consecutiveNewLinesCount>0){
          i++;
          //is there another "/" after this one?
          if(i<contentsLength && contents[i]=='/'){
            //skip the line
            consecutiveNewLinesCount--;
            for(; i<contentsLength && contents[i]!='\n'; i++);
          }else{
            i--;
          }
        }
        break;
          
      //===COMMENT Handler
      case '#':
        //are we at the start of a new line?
        if(consecutiveNewLinesCount>0){
          //skip the line
          consecutiveNewLinesCount--;
          for(; i<contentsLength && contents[i]!='\n'; i++);
        }
        break;
      
      //===ESCAPING Handler
      case '\\':
        i++;
        if(i>=contentsLength){
          break;
        }

        //this should be at least a TextBlock for the most part
        static_cast<TextBlock*>(textBlocks.back())->contents+=contents[i];
        break;
        
      
      //===SQUARE BRACKETS Handling
      case '[':
        i++;
        //make sure we are not at the end of the contents
        if(i>=contentsLength)
          break;
      
        switch(contents[i]){
          //===COLOR Handling
          //TODO make [#r] red, [#b] blue, [#y] yellow, [#o] orange, [#p] purple, [#g] green
          case '#':
            i++;
            //make sure that the amount of charcters (6) is correct, otherwise, invalid color, no need to parse
            if(i+6>=contentsLength || contents[i+6]!=']')
              break;

            //5th character is "o", most likely "defcol" (default color), no need to waste resources checking if it really is "defcol"
            if(contents[i+4]=='o'){
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

            //reset heading
            currentHeading=4;
            
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
        



            //is there no formating?
            if(consecutiveAsteriskCount+consecutiveCaretCount+consecutiveTildeCount+consecutiveUnderScoreCount==0){

              //did this line start with a X character
              switch(contents[i]){

                //===HEADING Handling
                case '!':
                  currentHeading=countConsecutiveCharactersBeforeSpace(contents, i, contentsLength, '!');

                  //if currentHeading is 0 make it 4, if not leave it alone
                  if(!currentHeading){
                    currentHeading=4;
                  }
                  break;

                //===QUOTE Handling
                case '>':
                  currentQuoteLevel=countConsecutiveCharactersBeforeSpace(contents, i, contentsLength, '>');
                  break;            
              }
            }
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
                    
          //===SUB-CRIPT handling
          if(consecutiveTildeCount==1){
            flags ^= TextBlock::fontFlags::SubScript;

          //===STRIKE-THROUGH handling
          }else if(consecutiveTildeCount>=2){
            flags ^= TextBlock::fontFlags::StrikeThrough;
          }

          //===SUPER-SCRIPT handling
          if(consecutiveCaretCount==1){
            flags ^= TextBlock::fontFlags::SuperScript;
          }
        
          static_cast<TextBlock*>(textBlocks.back())->fontFormat=flags;
          static_cast<TextBlock*>(textBlocks.back())->color=textColor;
          static_cast<TextBlock*>(textBlocks.back())->heading=currentHeading;
          static_cast<TextBlock*>(textBlocks.back())->quoteLevel=currentQuoteLevel;
          
        
          //reset these
          consecutiveAsteriskCount=0;
          consecutiveUnderScoreCount=0;
          consecutiveUnderScoreCount=0;
          consecutiveCaretCount=0;
          consecutiveTildeCount=0;
 
        }      
        
        //it it was a single line, the top didn't execute, and single newlines whould be ignored
        consecutiveNewLinesCount=0;
      
        startOfTextBlock=false;
        //this should be at least a TextBlock for the most part
        static_cast<TextBlock*>(textBlocks.back())->contents+=contents[i];
        break;
    }
      
  }
  
  */
}


void Document::setPreSetting(const std::string& in){


  //NOTE Perhaps I can make the whole contents processing only using regex
  std::smatch match;

  std::sregex_iterator regIterator(contents.begin(), contents.end(), preSettingPattern);
  std::sregex_iterator endIterator;

  for(; regIterator!=endIterator; regIterator++){
    match = *regIterator;

      //0 is whole string
      //1 is emphatic
      //2 is underlines
      //3 is bold
      //4 is italic
      //5 is strike through
      //6 is sub script
      //7 super script
      //8 is color in hex
      //9 is color in a single char
      //10 is color in word form
      //11 is 10 but just the word


    //TODO Check if the index stops where it sould actually stop
    int index{0};
    for(; regIterator->str(index)==""; index++);

    switch(index){
      case 0:
        //what?
        break;

      case 1:
        toggleEmphatic(preSetting.fontFormat);
        break;
  
      case 2:
        preSetting.fontFormat ^= TextBlock::fontFlags::Underlined;
        break;
  
      case 3:
        preSetting.fontFormat ^= TextBlock::fontFlags::Bold;
        break;

      case 4:
        preSetting.fontFormat ^= TextBlock::fontFlags::Italic;
        break;

      case 5:
        preSetting.fontFormat ^= TextBlock::fontFlags::StrikeThrough;
        break;

      case 6:
        preSetting.fontFormat ^= TextBlock::fontFlags::SubScript;
        break;

      case 7:
        preSetting.fontFormat ^= TextBlock::fontFlags::SuperScript;
        break;
  
      case 8:
            
        //reset the color
        preSetting.color=0x000000ff;

        // 012345678
        // [#ffffff]
        
        //add values to the color
        for(int j=0; j<6; j++){
          preSetting.color+=hexToDec(regIterator->str(8)[j+2])*std::pow(16, j+2);
        }
        break;    
      
      case 9:
        preSetting.color=charColorToHex(regIterator->str(9)[2]);
        break;

      case 10:
        preSetting.color=wordColorToHex(regIterator->str(11).c_str());
        break;
  
      default:
        //what?
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


int countConsecutiveCharactersBeforeSpace(const std::string& input, size_t& index, const std::size_t& size, const char& character){

    int output{0};
    std::size_t originalIndex=index;
  
    //we are calculating new heading
    for(;index<size && input[index]=='!'; index++){
      output++;
    }

    //is a space (" ") leading the character?
    if(input[index]==' '){
      index++;
      return output;

    //A space (" ") is not leading the character?
    }else{
      //reset the heading and index, as if nothing happened
      index=originalIndex;
      return 0;
    }
}

char* allTolower(char* in){
  for(size_t i{0}; in[i]!='\0'; i++){
    in[i]=std::tolower(in[i]);
  }
  return in;
}


uint32_t wordColorToHex(const char* in){

  //heh, const is no more
  allTolower((char*)in);

  if(std::strstr("red", in)!=nullptr){
    return 0xff000000; //red
  }else if(std::strstr("green", in)!=nullptr){
    return 0x00ff0000; //green
  }else if(std::strstr("blue", in)!=nullptr){
    return 0x0000ff00; //blue
  }else if(std::strstr("yellow", in)!=nullptr){
    return 0xffff0000; //yellow
  }else if(std::strstr("magenta", in)!=nullptr){
    return 0xff00ff00; //magenta
  }else if(std::strstr("cyan", in)!=nullptr){
    return 0x00ffff00; //cyan
  }else if(std::strstr("orange", in)!=nullptr){
    return 0xff7f0000; //orange
  }else if(std::strstr("purple", in)!=nullptr){
    return 0x7f00ff00; //purple
  }else if(std::strstr("turquoise", in)!=nullptr){
    return 0x00ff7f00; //turquoise
  }else if(std::strstr("lime", in)!=nullptr){
    return 0x7fff0000; //lime
  }else if(std::strstr("pink", in)!=nullptr){
    return 0xff007f00; //pink
  }else if(std::strstr("light blue", in)!=nullptr){
    return 0x007fff00; //light blue
  }else if(std::strstr("white", in)!=nullptr){
    return 0xffffff00; //white
  }else if(std::strstr("black", in)!=nullptr){
    return 0x00000000; //black
  }else if(std::strstr("gray", in)!=nullptr){
    return 0x7f7f7f00; //gray
  }else{
    //invalid color
    return 0x5F000000;
  
  }

}

uint32_t charColorToHex(const char& in){

  switch(in){
  case 'r':
    return 0xff000000; //red
  case 'g':
    return 0x00ff0000; //green
  case 'b':
    return 0x0000ff00; //blue
  case 'y':
    return 0xffff0000; //yellow
  case 'm':
    return 0xff00ff00; //magenta
  case 'c':
    return 0x00ffff00; //cyan
  case 'o':
    return 0xff7f0000; //orange
  case 'p':
    return 0x7f00ff00; //purple
  case 't':
    return 0x00ff7f00; //turquoise
  case 'l':
    return 0x7fff0000; //lime
  case 'P':
    return 0xff007f00; //pink
  case 'L':
    return 0x007fff00; //light blue
  case 'w':
    return 0xffffff00; //white
  case 'B':
    return 0x00000000; //black
  case 'G':
    return 0x7f7f7f00; //gray
  default:
    //invalid color
    return 0x5F000000;
  }

}

//this performed better, its essentially a map
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

