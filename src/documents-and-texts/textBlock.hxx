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



#include <cstdint>
#include <string>

/*
TODO make sure stuff like this is alos here:
bold
italic
emphasized (custom)
crossed
underlines
overlined?
subscript
superscript

*/

struct TextBlock{
  public:

    /*
      internal- another documnt
      website- some website
    */
    enum LinkType{internal, website};

    std::string contents{""};
    
    //r+g+b+a (should include (a)lpha?)
    uint32_t color{0x000000};
    
    bool isLink{false};
    LinkType linktype{internal};
    std::string link{""};
    
    int lineBreakLevel{0};
    
    bool isBold{false};
    bool isEmphatic{false};
    bool isUnderlined{false};
    bool isItalic{false};
    bool isSubScript{false};
    bool isSuperScript{false};
    bool isCrossedOut{false};//strike-through
    
    //TBD
    //differnt iszes should be on different lines?
    //keep them on the same line?
    int size{16};

};


