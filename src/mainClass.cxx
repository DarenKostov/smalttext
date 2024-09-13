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
#include <charconv>
#include <unistd.h>
#include <stdlib.h>


MainClass::MainClass(const std::filesystem::path& projectPath){
  window.create(sf::VideoMode(123, 123), "WXYZ");
  mainView=sf::View(sf::FloatRect(0, 0, 123, 123));
  mainView.setViewport(sf::FloatRect(0, 0, 1, 1));
  window.setView(mainView);
  window.setVerticalSyncEnabled(true);

  workingPath=projectPath;

  
}
MainClass::~MainClass(){
}



void MainClass::startProgram(){


  //load all documents magic here


  clock.restart();

  while(window.isOpen()){

    //drawing 66666-15fps 33333-30fps 16666-60fps 11111-90fps 8333-120fps 4166-240fps 
    usleep(16666);

    performActions();
    update();
    draw();


  }


}



void MainClass::loadFile(const std::filesystem::path&){

  //open file and parse it

  //get version

  //meta parse

  //parse

  //add document


}

