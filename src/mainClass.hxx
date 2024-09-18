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

    //font stuff
    enum typefaceEnum {Sans, Serif, Mono};
    enum fontFlagsEnum {Regular, Bold, Italic, Bold_Italic};
    std::unordered_map<typefaceEnum, std::unordered_map<fontFlagsEnum, sf::Font>> font;
    typefaceEnum chosenTypeFace{Serif};
    
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

    //loads all .smalttext files in the directory (resursive)
    void loadFilesIntoLibrary(const std::filesystem::path&);
    
    //loads just the meta stuff, loads it in to the library, meaning in makes a new document (tries to not cause duplicates)
    bool loadFileMeta(const std::filesystem::path&);

    //loads just the contents, MAKE SURE you have loaded the meta stuff (how else would you have a document either way)
    bool loadFileContents(Document*);

   
};
