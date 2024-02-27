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

//checks if 2 rectangles are colliding
bool areColliding(const int& x1, const int& y1, const int& w1, const int& h1, const int& x2, const int& y2, const int& w2, const int& h2);

void MainClass::performActions(){
  sf::Event event;
  while (window.pollEvent(event)){
    switch(event.type){
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::Resized:
        //if screen is resized fix the views dimensions
        {
          int windowWidth=event.size.width;
          int windowHeight=event.size.height;
          mainView.reset(sf::FloatRect(0.f, 0.f, windowWidth, windowHeight));

          //also read in the file again, update the memory document and finaly redraw it
          processTextBlockList(documentOnScreen);
          draw();
          
        }
        break;
      case sf::Event::MouseWheelMoved:
        {
          int scroll=event.mouseWheel.delta*30;
          
          movePage(scroll);
            
          //redraw everything with the new vertical offset
          draw();
        }

        break;
      case sf::Event::KeyPressed:
        switch(event.key.code){
          case sf::Keyboard::Up:
          case sf::Keyboard::PageUp:
          case sf::Keyboard::K:
            movePage(30);
            draw();
            break;
          case sf::Keyboard::Down:
          case sf::Keyboard::PageDown:
          case sf::Keyboard::J:
            movePage(-30);
            draw();
            break;
          default:
            //do nothing
            break;
        }
        break;
        case sf::Event::MouseButtonPressed:

          //go through all links on the current document
          for(const auto& [link, area] : documentTextLinks[documentOnScreen]){
            //if we are colliding with any link, open the document and stop checking for any other collisions
            auto properties=area.getGlobalBounds();
            if(areColliding(event.mouseButton.x, event.mouseButton.y, 1, 1, properties.left, properties.top, properties.width, properties.height)){
              std::cout << link->getTitle() << "Was clicked, should open that document!\n";
              break;
            }
          }
          break;
        case sf::Event::MouseMoved:
          break;
      // case blah blah:
      // / break;
    }
    
  }

}

bool areColliding(const int& x1, const int& y1, const int& w1, const int& h1, const int& x2, const int& y2, const int& w2, const int& h2){
	if (x2<=x1+w1) {
		if (x1<=x2+w2) {
			if (y2<=y1+h1) {
				if (y1<=y2+h2) {
					return true;
				}
			}
		}
	}
	return false;
}

void MainClass::movePage(const int& displacement){

  scrolledAmount[documentOnScreen]+=displacement;
  
  //move all text up/down
  for(auto& text : documentTexts[documentOnScreen]){
    text.move(0, displacement);
  }          

  //move all links up/down
  for(auto& link : documentTextLinks[documentOnScreen]){
    link.second.move(0, displacement);
  }          

}
