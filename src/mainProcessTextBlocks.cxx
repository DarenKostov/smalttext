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

#include "document/textBlock.hxx"
#include "mainClass.hxx"
#include <SFML/Graphics/Text.hpp>

void MainClass::processTextBlockList(Document* chosenOne){
  documentTexts[chosenOne].clear();
  
  auto textBlocks=chosenOne->getTextBlocks();

  double verticalOffset=0;
  double horizontalOffset=0;

  for(const auto& textBlock : textBlocks){
    //skip if nothing
    if(textBlock->contents.size()==0){
      continue;
    }
    sf::Text currentText;
    
    //handle new paragraphs
    if(textBlock->contents[0]=='\n'){
      horizontalOffset+=fontSize*fontSpaced;
    //handle new paragraph with newline
    }else if(textBlock->contents[1]=='\n'){
      horizontalOffset+=fontSize*fontSpaced*2;
    //TODO handle new section
    }else if(textBlock->contents[2]=='\n'){
      horizontalOffset+=fontSize*fontSpaced*3;
    }

    currentText.setString(textBlock->contents);

    if(textBlock->fontFormat &= FontFlags::CodeBlock){
      currentText.setFont(font[typefaceEnum::Mono][textBlock->fontFormat & (FontFlags::Italic | FontFlags::Bold)]);
    }else{
      currentText.setFont(font[typefaceEnum::Serif][textBlock->fontFormat & (FontFlags::Italic | FontFlags::Bold)]);
    }


  if(textBlock->fontFormat &= FontFlags::StrikeThrough){
    currentText.setStyle(sf::Text::StrikeThrough);
  }
  if(textBlock->fontFormat &= FontFlags::Underlined){
    currentText.setStyle(currentText.getStyle() | sf::Text::Underlined);
  }

  currentText.setFillColor(sf::Color(textBlock->color));
  
  //TODO
  /*
    handle headings
    handle code
  */
    

    documentTexts[chosenOne].push_back(currentText);
  }
  

}

