[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/trexem/WeNeedMoreAsteroids)

# ASTEROIDS GAME by trexem
Mostly testing and playing with SDL3, it is still WIP so be patient, I'm learning about it. I'm trying to achieve an ECS-Message Architecture.

* [AsteroidsGame.cpp](https://github.com/trexem/Asteroids/blob/master/src/AsteroidsGame.cpp) is the main file
* [EntityManager.hpp](https://github.com/trexem/Asteroids/blob/master/include/EntityManager.h) holds the logic for Entities Creation.
* [texture.hpp](https://github.com/trexem/Asteroids/blob/master/include/texture.hpp) has the Texture class, which is used to load images and text into the screen.
* [timer.hpp](https://github.com/trexem/Asteroids/blob/master/include/timer.hpp) has a simple timer class, you can pause, resume and restart timer.
* [game.hpp](https://github.com/trexem/Asteroids/blob/master/include/game.hpp) holds the methods for the gameLoop, ship creation and asteroids generation. I still need to clean and refactor it.
* [components.h](https://github.com/trexem/Asteroids/blob/master/include/Components.h) holds the Components that each entity can have.
* [systems](https://github.com/trexem/Asteroids/tree/master/include/systems) holds the multiple systems the game will have. They are responsible for all the logic on how the entities and components should behave.


## Try out
### Linux
Clone the repo with
```shell
git clone https://github.com/trexem/Asteroids.git --depth=1 --recurse-submodules"
``` 
so it downloads the SDL submodules. After that you might still have to download some files for SDL to compile correctly, you might have to run `download.sh` inside `external` subdirectory of SDL_image and SDL_ttf. 
To generate make files you need to run
```shell
cmake -S . -B build
```
Then to compile we run 
```shell
cmake --build build
```
Alternatively to generate build files and build you can just run
```shell
./build.sh
```
Then just open the asteroids program.
```shell
./build/asteroids
```
### Windows
Open `Visual Studio 2022` and clone the repo through the initial options. It should download the submodules and everything necessary for the project to compile. 
After it finishes downloading and `cmake` stops working you will have to choose `asteroids.exe` solution (besides the "Play" button). Then just press the "Play" button.

### Assets
Right now loading assets is being made trough a PackReader, I'm not adding the assets.pak as it would be redundant, but you might need to have that, so for now just zip 
the directories inside `data/` and rename that zip to `assets.pak` and save that in the main directory of the project.


## TODO
### Audio
 - [ ] Implement it
### Systems
 - [ ] EnemyAISystem (moves enemies based on AIComponent)
 - [ ] SpawnSystem (handles waves of enemies & asteroids)
### Abilities
 - [ ] TeslaCoil
 - [ ] Balance them
### Screens
 - [ ] GameOverScreen, add gold and kills display.
 - [ ] Settings
 - [ ] Upgrade Store
### Assets
 - [ ] Gold texture
 - [ ] Make it look good
### Upgrades
 - [ ] Implement them
### Good practices
 - [ ] Refactor all includes to forward declare and not include everything in header files
 - [ ] Decide and change all naming of the files to one standard (first letter mayus or not, hpp or h)
 
