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

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "documents-and-texts/document.hxx"

class MainClass{

  private:
    //variables:

    //for displaying;
    std::vector<sf::Text> currenltyDisplayed;
    Document* currentDocument;

    std::unordered_map<std::string, Document> documents;

    std::filesystem::path workingPath;
    

    //sfml stuff
    sf::RenderWindow window;
    sf::View mainView;
    sf::Clock clock;

    
    
  public:

    //constructor, like a atSTartUp function
    MainClass(const std::filesystem::path&);

    //deconstror
    ~MainClass();

    //the function that does all the logic, basically the main function
    void startProgram();

  
    private:
    
    //this performs tasks depending on the actions in the enviroment, like resizing the window 
    void performActions();
  
    //this updates the program, like a "loop" function
    void update();

    //this draws he program, like update but for drawing
    void draw();


    //TODO return error state
    //loads a file into the library
    void loadFile(const std::filesystem::path&);

   
};
