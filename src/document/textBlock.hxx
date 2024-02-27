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

/*
  Perhaps not the best way to accomplish what I am trying to do here
*/

#pragma once

#include <cstdint>
#include <filesystem>
#include <type_traits>

const uint32_t DEFAULT_TEXTBLOCK_COLOR{0x000000ff};

class Document;

//no fancy colors and font sizes, simple
struct TextBlock{
  enum type{Lite, Extended};
  
  virtual type whatAmI(){
    return Lite;
  }

  virtual ~TextBlock(){};

  enum fontFlags : unsigned int{
    Regular=0        << 0, //no flags
    Bold=1           << 0,
    Italic=1         << 1,
    Underlined=1     << 2,
    StrickeThrough=1 << 3,
    SubScript=1     << 4,
    SuperScript=1    << 5,
    CodeBlock=1      << 6,
  };      

  fontFlags fontFormat{Regular};
  std::string contents{""};

  Document* documentLink{nullptr};

};


//a text block with extended features
struct ExtendedTextBlock : public TextBlock{

  virtual type whatAmI(){
    return Extended;
  }

  int heading{4};
  uint32_t color{DEFAULT_TEXTBLOCK_COLOR};
  int quoteLevel{0};

  std::filesystem::path path{"/"};
  std::string url{""};

};
  

//https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c
//Bitwise OR
constexpr TextBlock::fontFlags operator|(const TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
  return static_cast<TextBlock::fontFlags>(std::underlying_type_t<TextBlock::fontFlags>(left) | std::underlying_type_t<TextBlock::fontFlags>(right));
}

//perform Bitwise OR, aka, add this flag, no matter if it is set ot not
inline TextBlock::fontFlags& operator|=(TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
  return left=left|right;
}

//Bitwise XOR
constexpr TextBlock::fontFlags operator^(const TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
  return static_cast<TextBlock::fontFlags>(std::underlying_type_t<TextBlock::fontFlags>(left) ^ std::underlying_type_t<TextBlock::fontFlags>(right));
}

//perform Bitwise XOR, aka, remove this flag if it there, add this flag if it not there
inline TextBlock::fontFlags& operator^=(TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
  return left=left^right;
}

//Bitwise AND
constexpr TextBlock::fontFlags operator&(const TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
  return static_cast<TextBlock::fontFlags>(std::underlying_type_t<TextBlock::fontFlags>(left) & std::underlying_type_t<TextBlock::fontFlags>(right));
}

//not what the &= operator normaly does
//checks if the flag or flags are included
inline bool operator&=(const TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
  return (left&right)==right;
}


