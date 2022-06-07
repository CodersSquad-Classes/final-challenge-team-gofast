#ifndef PACMAN_H

#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#define PACMAN_H
#define DOWN 2
#define UP 3
#define LEFT 4
#define RIGHT 5
#define XSIZE 28
#define YSIZE 31

#define NULL_COLOUR 0
#define WALL_COLOUR 1
#define PLAYER_COLOUR 2
#define BLINKY_COLOUR 3
#define PINKY_COLOUR 4
#define INKY_COLOUR 5
#define CLYDE_COLOUR 6
#define PELLET_COLOUR 7

//Initialise Functions
void buildMap(int logicalMap[][XSIZE], int ghostMap[][XSIZE]);
void initialisation(int logicalMap[][XSIZE],int ghostMap[][XSIZE]);
void eventHandler(int logicalMap[][XSIZE],int ghostMap[][XSIZE], int number_Ennemies);
void addCharAtPos(int y, int x, int colour, char icon);
void getFollowingCoord(int *y, int *x, int direction);
int getDirectionOpp(int direction);
int getDirectionSmart(int direction);
void GhostMotion(int *ghostY,int *ghostX, int *direction, int ghostColour, int playerY, int playerX,int logicalMap[][XSIZE],int ghostMap[][XSIZE], int *dead,int PelletCounter, int *score);
int getTraceRandomDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]);
int getRandomDirection();
void setLives(int lives);
void setMapChar(int Y, int X, char ch,int logicalMap[][XSIZE]);
char getPosOfChar(int y, int x, int logicalMap[][XSIZE]);
int getInverseRandomTraceDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]);
int getInverseTraceDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]);
int PlayerMotion(int *playerY, int *playerX, int direction, int *score, int logicalMap[][XSIZE],int *PowerPellet, int *remainingPellets);
int getTraceDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]);
int isRightPosition(int y, int x, int logicalMap[][XSIZE]);
int GhostInBox(int y, int x);

#endif