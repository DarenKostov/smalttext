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


#include "exporters.hxx"


bool convertToMarkdown(Document& document, std::string& output){

  auto& input{document.contents};

  for(auto& segment : input){

    //handle new paragraphs
    for(int i{0}; i<segment.lineBreakLevel; i++){
      output+="\n\n";
    }

    if(segment.isBold)
      output+="**";
    if(segment.isItalic)
      output+="*";
    if(segment.isCrossedOut)
      output+="~~";
    if(segment.isEmphatic)
      output+="";//NOTE THIS SHOULD BE CUSTOM
    if(segment.isSubScript)
      output+="~";
    if(segment.isSuperScript)
      output+="^";
    if(segment.isUnderlined)
      output+="";//no underlines in markdown :/
    
    output+=segment.contents;

    //make sure all of these are in reverse order

    if(segment.isUnderlined)
      output+="";//no underlines in markdown :/
    if(segment.isSuperScript)
      output+="^";
    if(segment.isSubScript)
      output+="~";
    if(segment.isEmphatic)
      output+="";//NOTE THIS SHOULD BE CUSTOM
    if(segment.isCrossedOut)
      output+="~~";
    if(segment.isItalic)
      output+="*";
    if(segment.isBold)
      output+="**";
  
  }  

  return true;

}

