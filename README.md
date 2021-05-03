# AgeOfBedWars
A console game inspired from the game Age of War and the minigame Bed Wars from minecraft.

Require CMake version 1.17 or more.  
Compiled and tested under linux with gcc version 7.5.0 and c++ 17.

# Installation
Before compiling run `cmake <pathToProject>` in a directory above the one that contains the project. On windows you need to add a corresponding `-G` parameter.  
Then run `make` to compile.

# Execution
Usage : `./AgeOfBedWars <backgroundFile> <baseHealth> <moneyPerTurn> [initialMoneyAmount]`  
`<arguments>` are required. `[arguments]` are optionnal.  
* `<backgroundFile>` : A **YAML** file containing the terrain parameters. See *resources/test.yaml* and *resources/test3pl.yaml* for file structure.  
  You can easily make your own game terrain by duplicating one of the examples and following the given comments.  
  All the extensions are accepted if intern file structure is correct. For more information on the file strucuture, see *src/Battlefield.cpp*.
* Other parameters are self explanatory, for more more infromations you will have to look at the code.

Recommended parameters are : `test.background 100 8 2`

# The Code
As this is part of my student work, I try to use a wide range of C++ features. Because of that I may code two things that has similar meaning in different ways.  
Besides that I try to use the c++ library and especially the algorithms library as often as possible. 

## Repository structure
* **includes** : *.hpp* files for sources and libraries sub directories have same structure as *src/* except for libraries folders.
* **libraries** : currently contains libyaml-cpp.a as a static library built for linux.
* **resources** : YAML files used to provide game information at startup.
* **src** : c++ sources with unit-related classes un *unit/* subdirectory.

## Code structure
* **Game.cpp** : Entry point, parse command arguments and contains the game loop.
* 
