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
class Document;

struct DocumentBlock{
  enum type{Separator, Text, DocumentLink, FileLink, UrlLink};
  virtual type whatAmI()=0;
};

struct SeparatorBlock : public DocumentBlock{
  enum separatorType{NewLine, NewParagraph, NewChapter};

  separatorType separator{NewLine};
    
  type whatAmI(){
    return Separator;
  }
};

struct TextBlock : public DocumentBlock{
  enum fontFlags{
    Regular=0        << 0, //no flags
    Bold=1           << 0,
    Italic=1         << 1,
    Underlined=1     << 2,
    StrickeThrough=1 << 3,
    SubScripts=1     << 4,
    SuperScript=1    << 5,
    CodeBlock=1      << 6,
  };      

  fontFlags fontFormat{Regular};
  int fontSize{13};
  uint32_t color{0x000000ff};
  std::string content{""};

  virtual type whatAmI(){
    return Text;
  }
      
};


struct TextBlockDocumentLink : public TextBlock{
  Document* link{nullptr};

  type whatAmI(){
    return DocumentLink;
  }
};

struct TextBlockFileLink : public TextBlock{
  std::filesystem::path path{"/"};

  type whatAmI(){
    return FileLink;
  }
};

struct TextBlockUrlLink : public TextBlock{
  std::string url{""};

  type whatAmI(){
    return UrlLink;
  }
};

