# AgeOfBedWars
A console game inspired from the game Age of War and the minigame Bed Wars from minecraft

Require CMake version 1.17 or more.
Compiled and tested under linux with gcc version 7.5.0 and c++ 17.

# Installation
Before compiling run `cmake <pathToProject>` in a directory above the one that contains the project.
On windows you need to add a corresponding `-G` parameter.
Then run `make` to compile.

# Execution
Usage : `./AgeOfBedWars <backgroundFile> <baseHealth> <moneyPerTurn> [initialMoneyAmount]`
<arguments> are required. [arguments] are optionnal.
  backgroundFile : A file containing the terrain parameters. See `Battlefield.cpp` for file structure. 
    A default file is provided is provided in resources/
    The file as extension .background but all the extensions are accepted if intern file structure is correct.
  Other parameters are self explanatory if you know the rules that I will not write dow for now.
  Recommended parameters are : `test.background 100 8 2`
