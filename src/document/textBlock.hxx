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

#pragma once

#include <cstdint>
#include <filesystem>
#include <type_traits>

const uint32_t DEFAULT_TEXTBLOCK_COLOR{0x000000ff};

class Document;

//is the font bold? underlined? etc?
enum class FontFlags : unsigned int{
  Regular=0        << 0, //no flags
  Bold=1           << 0,
  Italic=1         << 1,
  Underlined=1     << 2,
  StrikeThrough=1  << 3,
  SubScript=1      << 4,
  SuperScript=1    << 5,
  CodeBlock=1      << 6,
};      


//no fancy colors and font sizes, simple
//a text block with extended features
struct TextBlock{

  FontFlags fontFormat{FontFlags::Regular};
  std::string contents{""};

  Document* documentLink{nullptr};

  int heading{4};
  uint32_t color{DEFAULT_TEXTBLOCK_COLOR};
  int quoteLevel{0};

  std::filesystem::path path{"/"};
  std::string url{""};

};

//https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c
//Bitwise OR
constexpr FontFlags operator|(const FontFlags& left, const FontFlags& right){
  return static_cast<FontFlags>(std::underlying_type_t<FontFlags>(left) | std::underlying_type_t<FontFlags>(right));
}

//perform Bitwise OR, aka, add this flag, no matter if it is set ot not
inline FontFlags& operator|=(FontFlags& left, const FontFlags& right){
  return left=left|right;
}

//Bitwise XOR
constexpr FontFlags operator^(const FontFlags& left, const FontFlags& right){
  return static_cast<FontFlags>(std::underlying_type_t<FontFlags>(left) ^ std::underlying_type_t<FontFlags>(right));
}

//perform Bitwise XOR, aka, remove this flag if it's there, add this flag if it's not there
inline FontFlags& operator^=(FontFlags& left, const FontFlags& right){
  return left=left^right;
}

//Bitwise AND
//right (or left) serves as a "mask" of some sort
constexpr FontFlags operator&(const FontFlags& left, const FontFlags& right){
  return static_cast<FontFlags>(std::underlying_type_t<FontFlags>(left) & std::underlying_type_t<FontFlags>(right));
}

//not what the &= operator normaly does
//checks if the flag or flags are included
inline bool operator&=(const FontFlags& left, const FontFlags& right){
  return (left&right)==right;
}

// TODO
//not what the <=> operator normaly does
//checks if the flag or flags are included
// inline bool operator<=>(const TextBlock::fontFlags& left, const TextBlock::fontFlags& right){
//   return (left&right)==right;
// }

