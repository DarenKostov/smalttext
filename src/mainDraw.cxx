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

#include "mainClass.hxx"
#include <iostream>

void MainClass::draw(){

  std::cout << "Draw!!\n";

  //TODO perhaps make a tab menu on the top with all the documents loaded?

  
  //clear wondow and make sure we are drawing where we need to be drawing
  window.clear();
  window.setView(mainView);
  
  //set a background
  sf::RectangleShape background;
  background.setFillColor(sf::Color(0, 0, 0));
  background.setPosition(0, 0);
  background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
  window.draw(background);


  if(documentOnScreen!=nullptr){
    //draw all document texts of the current document
    for(const auto& text : documentTexts[documentOnScreen]){
      window.draw(text);
    }
  }

  window.display();
  
}
