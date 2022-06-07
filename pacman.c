#include "pacman.h"

int main(){

	//Set data collection arrays
	int logicalMap[YSIZE][XSIZE] = {0};
	int ghostMap[YSIZE][XSIZE] = {0};

	int number_Input;
	printf("How many ghosts do you want to fight ?\nPlease type a number between 1 and 4 : ");

	// As long as the user has not chosen between 1 and 4
	while (1)
	{
		// Taking the input of the user
		if (scanf("%d", &number_Input) != 1)
		{
			printf("How many ghosts do you want to fight?\nPlease type a number between 1 and 4 : ");
		}

		if (number_Input < 1 || number_Input > 4) {
			printf("The number must be between 1 or 4 : ");
			continue;
		}
		break;
	}


	//Initialise map, and start up curses screen
	initialisation(logicalMap,ghostMap);
	// Building the map
	buildMap(logicalMap,ghostMap);
	
	//Contails while loop, characteristics of players and ennemies and the threads. Handles events
	eventHandler(logicalMap,ghostMap, number_Input);

	// closing the window
	endwin();

}

//Sets player data back to default. 
void respawn(int *playerY, int *playerX, int *playerDirection, int *dead, int lives, int logicalMap[][XSIZE], int ghostMap[][XSIZE]){
	*playerY = 17;
	*playerX = 14;
	*playerDirection = 0;
	*dead = 0;
	setLives(lives);
}

//Put ghosts back to initial positions and get new directions
void respawnBlinky(int *blinkyY, int *blinkyX, int *blinkyDirection){
	*blinkyY = 15;
	*blinkyX = 12;
	*blinkyDirection = getRandomDirection();

}
void respawnPinky(int *pinkyY, int *pinkyX, int *pinkyDirection){
	*pinkyY = 15;
	*pinkyX = 15;
	*pinkyDirection = getRandomDirection();

}
void respawnInky(int *inkyY, int *inkyX, int *inkyDirection){
	*inkyY = 13;
	*inkyX = 12;
	*inkyDirection = getRandomDirection();

}
void respawnClyde(int *clydeY, int *clydeX, int *clydeDirection){
	*clydeY = 13;
	*clydeX = 15;
	*clydeDirection = getRandomDirection();

}

//Collects new inputs and directs where to send data
void eventHandler(int logicalMap[][XSIZE],int ghostMap[][XSIZE], int number_Ennemies){
	//Initialise game data


	// Using the threads, depending on how much ennemies the user asked before.
	// It allows to specify the number of threads that shoud execute the following block
	#pragma omp parallel num_threads(number_Ennemies + PLAYER_THREAD)
	{
		// omp master directive identifies a section of cade that must be run only by the master thread
		#pragma omp master
		{
			// Get the actual number of thread that are being used.
			int nb_thred = omp_get_thread_num();

			// Characteristics of the game
			char c = '\0';
			int counter = 0;
			int score = 0;
			int remainingPellets = 256;
			int dead = 0;
			int win = 0;
			int lives = 3;
			setLives(lives);

			//Initialise player data
			int playerY = 17;
			int playerX = 14;
			int playerDirection = 0;
			int PowerPellet = 0;
			int PelletCounter = 0;

			//Initialise data for ghosts
			int blinkyY = 15;
			int blinkyX = 12;
			int blinkyDirection = getRandomDirection();

			int pinkyY = 15;
			int pinkyX = 15;
			int pinkyDirection = getRandomDirection();

			int inkyY = 13;
			int inkyX = 12;
			int inkyDirection = getRandomDirection();

			int clydeY = 13;
			int clydeX = 15;
			int clydeDirection = getRandomDirection();

			// Whil we are in the game.
			while(c != 'q'){
					mvprintw(2,XSIZE+2,"Press q to quit");
				//Clears any characters that are waiting to be used. Prevents problems if a key is held down
				flushinp();
				//Set frequency at which loop will repeat
				usleep(250*1000);
				
				//Cause program to run without input
				nodelay(stdscr, 1);;
				cbreak();
				c=getch();

				//If the score is the maximum possible, then the player wins
				if(remainingPellets == 0){
					win = 1;
				}

				//If no input is collected, continue on current path, provided there are no blocks in the way
				if(c!= -1){
					//Check that next direction is valid.
					int lastDirection = playerDirection;
					int tempY = playerY;
					int tempX = playerX;
					getFollowingCoord(&tempY,&tempX,c);
					if(logicalMap[tempY][tempX] != 0){
					playerDirection = c;
					}
				}else{
					c = playerDirection;
				}

				//Check if game is running
				if(dead == 0 && win == 0){
					playerDirection = PlayerMotion(&playerY, &playerX, playerDirection, &score, logicalMap, &PowerPellet, &remainingPellets);
					if(PowerPellet == 1){
						PelletCounter = 38;	
						PowerPellet = 0;
					}
				}else{
					if(lives < 1){
					mvprintw(YSIZE/2,XSIZE/2-5,"Game over!");
					mvprintw(YSIZE/2+1,XSIZE/2-7,"Press f to exit");
					}
					addCharAtPos(playerY, playerX, PLAYER_COLOUR,'@');
				}
				//Handle ghost movement
				// Spawning ghost depending of the input of the user.
				switch(number_Ennemies){

					case 1:
						
						GhostMotion(&blinkyY, &blinkyX, &blinkyDirection,BLINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						break;
					case 2:
						GhostMotion(&blinkyY, &blinkyX, &blinkyDirection,BLINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						GhostMotion(&pinkyY, &pinkyX, &pinkyDirection,PINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						break;
					case 3:
						GhostMotion(&blinkyY, &blinkyX, &blinkyDirection,BLINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						GhostMotion(&pinkyY, &pinkyX, &pinkyDirection,PINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						GhostMotion(&inkyY, &inkyX, &inkyDirection,INKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						break;
					default:
						GhostMotion(&blinkyY, &blinkyX, &blinkyDirection,BLINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						GhostMotion(&pinkyY, &pinkyX, &pinkyDirection,PINKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						GhostMotion(&inkyY, &inkyX, &inkyDirection,INKY_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						GhostMotion(&clydeY, &clydeX, &clydeDirection,CLYDE_COLOUR,playerY,playerX,logicalMap,ghostMap,&dead,PelletCounter,&score);
						break;
				}
				
				if(PelletCounter > 0){

					PelletCounter-=1;

				}

				//If player is dead, and has no remaining lives...
				if(dead==1){
					if(lives < 1){
					mvprintw(YSIZE/2,XSIZE/2-5,"Game over!");
					mvprintw(YSIZE/2+1,XSIZE/2-7,"Press q to exit");
					}
					//Clear area where lives are printed. This prevents any characters from staying when they should not be there.
					mvprintw(0,XSIZE+2,"            ");
					lives = lives-1;
				}else if(win == 1){
					//Winning case
					mvprintw(YSIZE/2,XSIZE/2-15,"Congratulations! You have won!");
					mvprintw(YSIZE/2+1,XSIZE/2-7,"Press q to exit");
				}
				//If player dead, and has remaining lives, respawn entities
				if(dead == 1 && lives> 0){
					respawn(&playerY, &playerX, &playerDirection, &dead, lives, logicalMap,ghostMap);
				}
			}
		}
		
	}

	
}

void respawnGhostFromColour(int colour, int *ghostY, int *ghostX, int *direction){
	int y = *ghostY;
	int x = *ghostX;
	int d = *direction;
	if(colour == BLINKY_COLOUR){
		respawnBlinky(&y,&x,&d);
	}else if(colour == PINKY_COLOUR){
		respawnPinky(&y,&x,&d);
	}else if(colour == INKY_COLOUR){
		respawnInky(&y,&x,&d);
	}else if(colour == CLYDE_COLOUR){
		respawnClyde(&y,&x,&d);
	}
	*ghostY = y;
	*ghostX = x;
	*direction = d;
}

void GhostMotion(int *ghostY,int *ghostX, int *direction, int ghostColour, int playerY, int playerX,int logicalMap[][XSIZE],int ghostMap[][XSIZE], int *dead, int PelletCounter, int *score){
	//Initialise new varibles to that chances are limited to this function.
	int y = *ghostY;
	int x = *ghostX;
	int oldGhostColour = ghostColour;
	//Remove ghost character
	//addCharAtPos(y,x,NULL_COLOUR,getPosOfChar(y,x,logicalMap));
	int tempDirection = *direction;
	int validMove = 0;
	int vulnerable = 0;
	int ghostDead = 0;
	//eating option
	if(PelletCounter > 0){
		vulnerable = 1;
		ghostColour = NULL_COLOUR;
		//2/3 chance of changing direction
			int random = rand()%3+1;
			if(random != 1){
				tempDirection = getInverseRandomTraceDirection(*direction, y,x,playerX,playerY,logicalMap);
			}
	}
	
	//Check if ghost is at player coordinates
	if(x == playerX && y == playerY){
		if(vulnerable == 0){
		*dead = 1;
		}else{
			respawnGhostFromColour(oldGhostColour,&y,&x,&tempDirection);
			validMove = 1;
			ghostDead = 1;
			*ghostX = x;
			*score = *score + 300;
			*ghostY = y;
			*direction = tempDirection;
			return;
		}
	}
	//Check if the move is valid. If not, try again until it is.
	while(validMove == 0){
		//If point is a intersection...
		if(ghostMap[y][x]==3){
			//2/3 chance of changing direction
			int random = rand()%3+1;
			if(random != 1){
				if(vulnerable == 0){
				tempDirection = getTraceRandomDirection(*direction,y,x,playerY, playerX,logicalMap);
				}else{
					tempDirection = getInverseRandomTraceDirection(*direction, y,x,playerX,playerY,logicalMap);
				}
			}
		}
		addCharAtPos(*ghostY,*ghostX,PELLET_COLOUR,getPosOfChar(*ghostY,*ghostX,logicalMap));
		//Try and move towards player
		int tempY = y;
		int tempX = x;
		getFollowingCoord(&tempY,&tempX,tempDirection);
		if(logicalMap[tempY][tempX] != 0){
			validMove = 1;
			y = tempY;
			x = tempX;

		}else{
			int random = rand()%3+1;
			if(random != 1){
				if(vulnerable == 0){
				tempDirection = getTraceRandomDirection(*direction,y,x,playerY, playerX,logicalMap);
				}else{
				tempDirection = getInverseRandomTraceDirection(*direction, y,x,playerX,playerY,logicalMap);
				}
		}
	}


	//If at exit of box, move up.
	if(validMove == 1){
		if(ghostMap[y][x] == 2){
			int random = rand()%4+1;
			if(random != 2){
				tempDirection = UP;
			}
		}
	}

	//Print ghost character
	addCharAtPos(y,x,ghostColour,'X');
	//Check if player is dead. If he is, move randomly. 
	if(*dead == 1){
		tempDirection == getDirectionSmart(*direction);
	}
	//assign varibles back to event handler
	*direction = tempDirection;
	*ghostY = y;
	*ghostX = x;
	//check again if ghost is at player location
	if(x == playerX && y == playerY){
		if(vulnerable == 0){
		*dead = 1;
		}else{
			respawnGhostFromColour(oldGhostColour,&y,&x,&tempDirection);
			validMove = 1;
			ghostDead = 1;
			*ghostX = x;
			*ghostY = y;
			*score = *score + 300;
			*direction = tempDirection;
			return;
		}
	}
}
	
}

//Get direction that moves towards player
int getTraceDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]){
	//Get distance between x and y of player and ghost
	int prefferedDirection = 0;
	int bothX = ghostX-playerX;
	int bothY = ghostY-playerY;
	//If ghost is in starting box, don't track player
	if (GhostInBox(ghostY,ghostX)){
		return getDirectionSmart(direction);
	}
	//Pythagorean theorem
	float distance = sqrt(((bothX*bothX)+(bothY*bothY)));

	//Try to find valid move towards player. It should almost never reach 25.
	int i;
	for(i=0;i<25;i++){
		int tempDirection = getRandomDirection();
		int tempY = ghostY;
		int tempX = ghostX;
		getFollowingCoord(&tempY,&tempX,tempDirection);
		int bothX2 = tempX - playerX;
		int bothY2 = tempY - playerY;
		float distance2 = sqrt((bothX2*bothX2)+(bothY2*bothY2));

		if(distance2<distance){
			return tempDirection;
		}
	}

	//If tracking direction not found, default to a more random one.
	return getDirectionSmart(direction);
}

//Get direction that moves away from player
int getInverseTraceDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]){
	//Get distance between x and y of player and ghost
	int prefferedDirection = 0;
	int bothX = ghostX-playerX;
	int bothY = ghostY-playerY;
	//If ghost is in starting box, don't track player
	if (GhostInBox(ghostY,ghostX)){
		return getDirectionSmart(direction);
	}
	//Pythagorean theorem
	float distance = sqrt(((bothX*bothX)+(bothY*bothY)));

	//Try to find valid move away from player. Loop should almost never reach 25.
	int i;
	for(i=0;i<25;i++){
		int tempDirection = getRandomDirection();
		int tempY = ghostY;
		int tempX = ghostX;
		getFollowingCoord(&tempY,&tempX,tempDirection);
		int bothX2 = tempX - playerX;
		int bothY2 = tempY - playerY;
		float distance2 = sqrt((bothX2*bothX2)+(bothY2*bothY2));

		if(distance2>distance){
			return tempDirection;
		}
	}


	//If inverse tracking direction not found, default to a more random one.
	return getDirectionSmart(direction);
}

//Get tracking direction 2/3 of the time. 1/3 of time just get random direction
int getTraceRandomDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]){
	int random = rand()%3+1;
	if(random!= 1){
		return getTraceDirection(direction,ghostY,ghostX,playerY,playerX,logicalMap);
	}else{
		return getDirectionSmart(direction);
	}
}

//Get invserse tracking direction 2/3 of the time. 1/3 of time just get random direction
int getInverseRandomTraceDirection(int direction, int ghostY, int ghostX, int playerY, int playerX,int logicalMap[][XSIZE]){
	int random = rand()%3+1;
	if(random!= 1){
		return getInverseTraceDirection(direction,ghostY,ghostX,playerY,playerX,logicalMap);
	}else{
		return getDirectionSmart(direction);
	}
}

//Check if entity is in box
int GhostInBox(int y, int x){
	if((y >= 13 && y <= 15)&&(x>=10 &&x<=16)){
		return 1;
	}
	return 0;
}
//Check if position is valid. i.e. not inside a wall
int isRightPosition(int y, int x, int logicalMap[][XSIZE]){
	if(logicalMap[y][x] == 0){
		return 0;
	}

	return 1;
}

//Get random direction, but decrease the chance of turning around.
int getDirectionSmart(int direction){
	int random = rand()%3+1;

	int newDirection = getRandomDirection();

	if((direction == newDirection || direction == getDirectionOpp(newDirection)) && random != 3){
		int i = 0;
		for(i = 0;i<5;i++){
			newDirection = getRandomDirection();
			if(direction != newDirection && direction != getDirectionOpp(newDirection)){
				return newDirection;
			}
		}
	}
	return newDirection;

}

//Pretty self-explanatory. Returns the opposite direction as the input.
int getDirectionOpp(int direction){
	if(direction == UP){
		return DOWN;
	}else if(direction == DOWN){
		return UP;
	}else if(direction == LEFT){
		return RIGHT;
	}else if(direction == RIGHT){
		return LEFT;
	}
}

//Set the display for lives
void setLives(int lives){
	int i;
	mvprintw(0,XSIZE+2,"            ");
	for(i=0;i<lives-1;i++){
		attron(COLOR_PAIR(PLAYER_COLOUR));
		mvaddch(0,XSIZE+2+i,'@');
		attroff(COLOR_PAIR(PLAYER_COLOUR));
	}
}

//Move the player's location based on the direction. Update score if pellet is consumed
int PlayerMotion(int *playerY, int *playerX, int direction, int *score, int logicalMap[][XSIZE], int *PowerPellet, int *remainingPellets){
	//set variables
	int y = *playerY;
	int x = *playerX;
	int tempY = y;
	int tempX = x;
	int tempDirection = direction;

	//clear last character
	logicalMap[y][x] = 1;
	addCharAtPos(y,x,NULL_COLOUR,' ');


	// Direction of the player.	
	switch(direction){
		case UP:
		if(logicalMap[y-1][x] != 0){
			y--;
		}
		break;
		case DOWN:
			if(logicalMap[y+1][x] != 0){
			y++;
		}
		break;
		case LEFT:
			if(logicalMap[y][x-1] != 0){
				x--;
			}
		break;
		case RIGHT:
		if(logicalMap[y][x+1] != 0){
			x++;
		}
		break;
	}
	if(x==0 && direction == LEFT){
		x = XSIZE;
	}else if(x==XSIZE-1 && direction == RIGHT){
		x = 0;
	}
	//Check if pellet has been eaten
	if(logicalMap[y][x] == 2){
		*score = *score + 10;
		*remainingPellets = *remainingPellets-1;
		logicalMap[y][x] = 1;
	}if(logicalMap[y][x] == 3){
		logicalMap[y][x] = 1;
		*score = *score + 100;
		*remainingPellets = *remainingPellets-1;
		*PowerPellet = 1;
		
	}



	//add player character
	attron(COLOR_PAIR(PLAYER_COLOUR));
	mvaddch(y,x,'@');
	attroff(COLOR_PAIR(PLAYER_COLOUR));
	//Update score info.
	char ScoreInfo[32] = "";
	sprintf(ScoreInfo,"Score: %d",*score);
	attron(COLOR_PAIR(BLINKY_COLOUR));
	mvprintw(1,XSIZE+2,ScoreInfo);
	attroff(COLOR_PAIR(BLINKY_COLOUR));
	//Update player data
	*playerY = y;
	*playerX = x;
	return direction;

}
//Get next coordinates for entity
void getFollowingCoord(int *yPos, int *xPos, int direction){
		switch(direction){
		case UP:
		*yPos = *yPos - 1;
		break;
		case DOWN:
		*yPos = *yPos + 1;
		break;
		case LEFT:
		*xPos = *xPos - 1;
		break;
		case RIGHT:
		*xPos = *xPos + 1;
		break;
	}
	if(*xPos==0 && direction == LEFT){
		*xPos = XSIZE;
	}else if(*xPos==XSIZE-1 && direction == RIGHT){
		*xPos = 0;
	}
	

}

//Initialise colours, random, and curses screen
void initialisation(int logicalMap[][XSIZE],int ghostMap[][XSIZE]){
	keypad(initscr(),TRUE);
  	curs_set(0);

  	srand(time(0));

  	//initialise colours
	start_color();
	init_pair(WALL_COLOUR,COLOR_BLUE,'\0');
	init_pair(PLAYER_COLOUR,COLOR_YELLOW,'\0');
	init_pair(NULL_COLOUR,COLOR_WHITE,'\0');
	init_pair(BLINKY_COLOUR,COLOR_RED,'\0');
	init_pair(PINKY_COLOUR,COLOR_MAGENTA,'\0');
	init_pair(INKY_COLOUR,COLOR_CYAN,'\0');
	init_pair(CLYDE_COLOUR,COLOR_GREEN,'\0');
	init_pair(PELLET_COLOUR,COLOR_YELLOW, '\0');
	clear();

	
}

//Get fully random direction
int getRandomDirection(){
	return rand()%4+2;
}
//Adds a coloured character at a specified location
void addCharAtPos(int y, int x, int colour, char icon){
		attron(COLOR_PAIR(colour));
		mvaddch(y,x,icon);
		attroff(COLOR_PAIR(colour));
}

//Uses logicalMap to find what character is on the map at a specified location.
char getPosOfChar(int y, int x, int logicalMap[][XSIZE]){
	if(logicalMap[y][x] == 1){
		return ' ';
	}else if(logicalMap[y][x] == 2){
		return '.';
	}else if(logicalMap[y][x] == 3){
		return 'O';
	}
	return ' ';
}


//Print map character
void setMapChar(int Y, int X, char ch, int logicalMap[][XSIZE]){
	//If ghost information, replace it.
	if(ch == 'T'){
		ch = '.';
	}else if(ch == 't' || ch == 'E' || ch == 'U'){
		ch = ' ';
	}

	//Add map character
	if(ch == '.' && logicalMap[Y][X] != 1){
		attron(COLOR_PAIR(NULL_COLOUR));
		mvaddch(Y,X,ch);
		attroff(COLOR_PAIR(NULL_COLOUR));
	}else if(ch == '#'){
		attron(COLOR_PAIR(WALL_COLOUR));
		mvaddch(Y,X,ch);
		attroff(COLOR_PAIR(WALL_COLOUR));
	}else if(ch == ' '){
		attron(COLOR_PAIR(NULL_COLOUR));
		mvaddch(Y,X,' ');
		attroff(COLOR_PAIR(NULL_COLOUR));
	}else if((ch == '.' || ch == 'O') && logicalMap[Y][X] == 1 ){
		attron(COLOR_PAIR(PELLET_COLOUR));
		mvaddch(Y,X,' ');
		attroff(COLOR_PAIR(PELLET_COLOUR));
	}else if(ch == 'O' && (logicalMap[Y][X] == 3) || logicalMap[Y][X] == 0){
		attron(COLOR_PAIR(PELLET_COLOUR));
		mvaddch(Y,X,'O');
		attroff(COLOR_PAIR(PELLET_COLOUR));
	}

}


void buildMap(int logicalMap[][XSIZE], int ghostMap[][XSIZE]){
	char map[YSIZE][XSIZE]   = {"############################",
								"#.....T......##......T.....#",
								"#.####.#####.##.#####.####.#",
								"#O#  #.#   #.##.#   #.#  #O#",
								"#.####.#####.##.#####.####.#",
								"#T....T..T..T..T..T..T....T#",
								"#.####.##.########.##.####.#",
								"#.####.##.########.##.####.#",
								"#.....T##....##....##T.....#",
								"######.#####.##.#####.######",
								"     #.#####.##.#####.#     ",
								"     #.##....EE....##.#     ",
								"     #.##.###  ###.##.#     ",
								"######.##.#  UU  #.##.######",
								"      T..T#      #T..T      ",
								"######.##.#      #.##.######",
								"     #.##.########.##.#     ",
								"     #.##T........T##.#     ",
								"     #.##.########.##.#     ",
								"######.##.########.##.######",
								"#.....T..T...##...T..T.....#",
								"#.####.#####.##.#####.####.#",
								"#O####.#####.##.#####.####O#",
								"#...##T..T..T..T..T..T##...#",
								"###.##.##.########.##.##.###",
								"###.##.##.########.##.##.###",
								"#..T...##....##....##...T..#",
								"#.##########.##.##########.#",
								"#.##########.##.##########.#",
								"#...........T..T...........#",
								"############################"};
			//Add data to ghost map
			int y;
			for(y=0;y<YSIZE;y++){
				int x;
				for(x=0;x<XSIZE;x++){

					if(map[y][x] == 'T'){
						ghostMap[y][x] = 3;
						map[y][x] = '.';
					}else if(map[y][x] == 'E'){
						ghostMap[y][x] = 1;
						map[y][x] = ' ';
					}else if(map[y][x] == 'U'){
						ghostMap[y][x]= 2;
						map[y][x]=' ';
					}
					//Print characters and add data to logical map.
					if(map[y][x] == '#'){
						attron(COLOR_PAIR(WALL_COLOUR));
						logicalMap[y][x]=0;
						mvaddch(y,x,map[y][x]);
						attroff(COLOR_PAIR(WALL_COLOUR));
					}else if(map[y][x] == ' '){
						attron(COLOR_PAIR(WALL_COLOUR));
						mvaddch(y,x,map[y][x]);
						attroff(COLOR_PAIR(WALL_COLOUR));
						logicalMap[y][x] = 1;
					}else if(map[y][x] == '.' && logicalMap[y][x] != 1){
						attron(COLOR_PAIR(PELLET_COLOUR));
						mvaddch(y,x,map[y][x]);
						logicalMap[y][x] = 2;
						attroff(COLOR_PAIR(PELLET_COLOUR));
					}else if(map[y][x] == 'O'){
						mvaddch(y,x,map[y][x]);
						logicalMap[y][x] = 3;
					}
				}
			}
			//Remove dot from logical array to prevent score from starting at 10.
			logicalMap[23][14] = 1;

		}