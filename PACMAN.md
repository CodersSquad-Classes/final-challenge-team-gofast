# Pacman

This is an implementation of the original PacMan game using a multithreaded approach.
Each enemy is an independent thread and the number of enemies is configurable.

![capture](./assets/diagrams/game-capture.png)

## Tech Stack

The project was built using C by displaying the game in the terminal.

## Technical Requirements

* The game's maze layout can be static.
* The pacman gamer must be controlled by the user.
* Enemies are autonomous entities that will move a random way.
* Enemies and pacman should respect the layout limits and walls.
* Enemies number can be configured on game's start.
* Each enemy's behaviour will be implemented as a separated thread.
* Enemies and pacman threads must use the same map or game layout data structure resource.
* Display obtained pacman's scores.
* Pacman loses when an enemy touches it.
* Pacman wins the game when it has taken all coins in the map.

## System Requirements

This project uses go modules, so Ebiten will be installed automatically as
you build the project. For Ebiten to work, you'll need to have installed:

* [C](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170) version C.17 or above
* C compiler (Only if you are using MacOS or Linux)
* Depending on which platform you are using, you might need to install some extra dependencies.

> We tested the project in MacOS and Ubuntu.

## Build/Run

### First build, then run

To build the executable, run:

```bash
$ make pacman
```

Then, run it with:

```bash
$ ./pacman
```

To specify the number of enemies this message will appear in the terminal:

```bash
$ How many ghosts do you want to fight?
$ Please type a number between 1 and 4 : 
 ```

> The maximum number of enemies allowed is 4 because the game becomes too difficult afterwards.

## Architecture

Visit the architecture document [here](./ARCHITECTURE.md).

## Video Presentation

You can check out the video [here](https://youtu.be/aHMOOvbfAsE).