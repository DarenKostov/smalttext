#include "mainClass.hxx"

void MainClass::draw(){


  //TODO perhaps make a tab menu on the top with all the documents loaded?

  
  //clear wondow and make sure we are drawing where we need to be drawing
  window.clear();
  window.setView(mainView);
  
  //set a background
  sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
  background.setFillColor(sf::Color(255, 255, 255));
  background.setPosition(0, 0);
  window.draw(background);


  //draw all document texts of the current document
  for(const auto& text : documentTexts[documentOnScreen]){
    window.draw(text);
  }


}
