
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unistd.h>
using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid 30
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid 30
	    
	    
// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;
//these variables are later to be moved in the main function as the global varaibles are not allowed
const int xDir=3;
const int yDir=4;
const int ifHead=5;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void movSprite(sf::Clock& bulletClock,float player[], int&,int&,int centipede[][6]);
// 1 this function helps in the movement of sprite as well as the collion between player and the centipede.

void shot(float bullet[],float player[]); 
// 2 This function allows to shot the bullets on key press

void drawMushroom(sf::RenderWindow& window, float mushroom[][gameColumns], sf::Sprite& mushroomSprite,int& mBreakx,int& mBreaky,int& score); 
//3 This function draws Mushrooms and also their stages while being hit by the laser

int setMushroom( float mushroom[][gameColumns]);
//4. This function initializes the position of mushrooms

void drawCrHead(sf::RenderWindow& window,sf::Sprite& bodySprite,  sf::Sprite& crHeadSprite,int head[],int centipede[][6]);
//5. This function draws the whole centipede and also check is if centipede is hit by laser



void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
//6. This function draws the player 

void moveBullet(float bullet[], sf::Clock& bulletClock,int&,int&,int&,int&,int centipede[][6],int& score);
//7 The bullet movement is so complex part. This function not only moves the bullet but contains checks which detects the collsions between mushroom and centipede

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
// This functino just draw the bullets where it exists.

void printGameGrid();
// This function is one of the most important function that really helped me. Debugging became very easy after I came to know about this thing. this function actually prints the game grid.

void sethead(int centipede[][6], int head[],sf::Clock& bodyClock,int size,int nMushrooms);
// This function initializes the postion of centipede. 


void wave(int centipede[][6],int head[],sf::Clock& centiClock);
// This function controls the complex movement of centipede. Moves the centipede according to rules. Also the centipede vibrates in the player area.

int main()
{	


	
	//////////////////////////////////////////////////
	//checking the values of game grid
	cout<<"grid:"<<endl
	    <<"resolutionX: " << resolutionX<<endl
	    <<"resolutionY: " <<resolutionY <<endl
	    <<"boxPixelsX: " <<boxPixelsX <<endl
	    << "boxPixelsY: "<<boxPixelsY <<endl
	    << "gameRows: "<<gameRows <<endl
	    << "gameColumns: "<<gameColumns <<endl;
	///////////////////////////////////////////////////////
	
	
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX-32, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.4)); // Reduces Opacity to 25%
	
	sf::Texture startTexture;
	sf::Clock startClock;
	sf::Sprite startSprite;
	startTexture.loadFromFile("Textures/start.png");
	startSprite.setTexture(startTexture);
	
	
	sf::Texture overTexture;
	sf::Sprite overSprite;
	overTexture.loadFromFile("Textures/over.png");
	overSprite.setTexture(overTexture);
	overSprite.setColor(sf::Color(255, 255, 255, 255 * 0.4)); // Reduces Opacity to 25%
	overSprite.setPosition(240,240);
	// Initializing Player and Player Sprites.
	float player[3] = {};
	player[x] = (gameColumns / 2)*boxPixelsX;
	player[y] = 800;
	player[exists]=1;
	sf::Clock playerClock;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY/2-2;
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
	int bx,by;
	//////////////////////////////////////////////////////////////////////////
	//mushroom declaration
	
	float mushroom[gameRows][gameColumns];
	int nMushrooms=setMushroom(mushroom);//calling function to set the postion of  Mushrooms 
	int mBreakx=0,mBreaky=0;
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(mBreakx , mBreaky , boxPixelsX, boxPixelsY));
	
	
	int px,py; //declaring variables to compare postion of mushroom on game grid
	//////////////////////////////////////////////////////////////////////////
	
	int centipede[12][6];
	sf::Clock centiClock;
	//////////////////////////////////////////////////////////////////////////
	//centiped's right head declaration
	int head[4];
	sf::Clock headClock;
	sf::Texture centipedeTexture;
	sf::Sprite centipedeSprite;
	centipedeTexture.loadFromFile("Textures/crHead.png");
	centipedeSprite.setTexture(centipedeTexture);
	int AnimationHeadX,AnimationHeadY;//Setting texture of Head to start. It'll help in animation
	AnimationHeadX=AnimationHeadY=0;
	centipedeSprite.setTextureRect(sf::IntRect(AnimationHeadX , AnimationHeadY , boxPixelsX-4, boxPixelsY));
	
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//centiped's right head declaration
	
	sf::Clock bodyClock;
	sf::Texture bodyTexture;
	sf::Sprite bodySprite;
	bodyTexture.loadFromFile("Textures/crBody.png");
	bodySprite.setTexture(bodyTexture);\
	int AnimationBodyX,AnimationBodyY;//Setting texture of body to start. It'll help in animation
	AnimationBodyX=AnimationBodyY=0;
	bodySprite.setTextureRect(sf::IntRect(AnimationBodyX , AnimationBodyY , boxPixelsX-4, boxPixelsY));
	int size;// size of centipede segments
	//////////////////////////////////////////////////////////////////////////
	
	////////////
	//head[x]=15;
	//head[y] = 0;
	//head[2] = 0;
	
	sethead(centipede,head,bodyClock,size,nMushrooms);//clock[2] is for  setting the intial postion 
	int score=0;
	bool start=1;
	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////
		
		
			if(start)
			window.draw(startSprite);
			
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			start=0;
		if(!start)
		{
		wave(centipede,head,centiClock);
		
		//Calling the function to move the sprite and shot bullet
		movSprite(playerClock,player,px,py,centipede);
		shot(bullet,player);
		
		
		 
		
		
		
		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);
		
			
		drawCrHead(window ,bodySprite, centipedeSprite,head,centipede);
		
		drawMushroom(window, mushroom, mushroomSprite,mBreakx,mBreaky,score);
		//dataMushroom();
		
		printGameGrid(); //the function to print the array of game grid
		
		if (bullet[exists] == true) {
			moveBullet(bullet, bulletClock,bx,by,mBreakx,mBreaky,centipede,score);
			drawBullet(window, bullet, bulletSprite);
		}
		
		}		
		if(player[exists]==0)
		
		window.draw(overSprite);
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			cout<<"Your scores are: "<<score;
				return 0;
			}
		}		
		window.display();
		window.clear();
		
	}
	
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
//Initializing postion of  mushrooms
int setMushroom( float mushroom[][gameColumns]) 
{
	int nMushrooms=20+rand()%11+5; //defining number of mushroom between 20 and 30
	
	for(int i=0; i<nMushrooms ; i++)
	{
		int x=rand()%gameRows;
		int y=rand()%27;
		
		mushroom[x][y]=6;
	}
	
	for(int i=0; i < gameRows; i++)
	{
			for(int j=0; j < gameColumns; j++)
			{
				if(mushroom[i][j]==6 ) 
				{
					gameGrid[i][j]=3;
					
				}
			}
	}
	return nMushrooms;
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
//drawing mushroom
void drawMushroom(sf::RenderWindow& window, float mushroom[][gameColumns], sf::Sprite& mushroomSprite,int& mBreakx,int& mBreaky,int& score) 
{
	
	
	for(int i=0; i < gameRows; i++)
		{
			for(int j=0; j < gameColumns; j++)
			{
				//if(mushroom[i][j]==6 ) 
				{	
					
					if(gameGrid[i][j]==3)
					{
						mushroomSprite.setPosition(i*32, j*32);
						mushroomSprite.setTextureRect(sf::IntRect(0 , 0 , boxPixelsX, boxPixelsY));
						window.draw(mushroomSprite);
					}
					if(gameGrid[i][j]==2)
					{
						gameGrid[i][j]--;
						
						mushroomSprite.setPosition(i*32, j*32);
						mushroomSprite.setTextureRect(sf::IntRect(32 , 0 , boxPixelsX, boxPixelsY));
						window.draw(mushroomSprite);
					}
					if(gameGrid[i][j]==1)
					{
						mushroomSprite.setPosition(i*32, j*32);
						mushroomSprite.setTextureRect(sf::IntRect(64 , 0 , boxPixelsX, boxPixelsY));
						window.draw(mushroomSprite);
						
					}
					
					
				}
			}
			
	
	}
	
	
	
mushroomSprite.setTextureRect(sf::IntRect(0 , 32 , boxPixelsX, boxPixelsY));
	
}
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
//function that shots the fire.
void shot(float bullet[],float player[])
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet[exists] && player[exists]) {
           bullet[x] = player[x];
            bullet[y] = player[y] - boxPixelsY; //first sets all the value of bullet coordinates above the sprite
            bullet[exists] = true; //turn the existance true if space is pressed
            
        }


}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//Function that moves the sprite
void movSprite(sf::Clock& playerClock,float player[],int& px, int& py,int centipede[][6]){ 
					//passing px and py by reference

px=player[x]/32;
py=player[y]/32;

if (playerClock.getElapsedTime().asMilliseconds() < 60) 
	return;
playerClock.restart();
	
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)  &&  player[x] > 0 && gameGrid[px-1][py]!=3)
	
	player[x]-=boxPixelsX;
		
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX-2*boxPixelsX && gameGrid[px+1][py]!=3) 
	player[x]+=boxPixelsX;
	
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&  player[y] > 24*32 && gameGrid[px][py-1]!=3)
	 player[y]-=boxPixelsY;
	
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&  player[y] < resolutionY-boxPixelsY && gameGrid[px][py+1]!=3)
	player[y]+=boxPixelsX;
	
for(int i=0; i<12; i++)
	{
		if(centipede[i][x]==player[x]/32 && centipede[i][y]==player[y]/32)
		{
			player[exists]=0;
			
		}
	}
	
}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//displaying array of game grid
void printGameGrid() {
    for (int i = 0; i < gameRows; ++i) {
        for (int j = 0; j < gameColumns; ++j) {
            std::cout << gameGrid[j][i] << " ";
            
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
///////////////////////////////////////////////////////////////////////////////////


//........................Pre-defined..............................
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	if( player[exists])
	{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
	}
}
void moveBullet(float bullet[], sf::Clock& bulletClock,int& bx,int& by,int& mBreakx,int& mBreaky, int centipede[][6], int& score) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;
	bulletClock.restart();
	
	bullet[y] -= boxPixelsY;	
	bx=bullet[x]/32;
	by=bullet[y]/32;
	
	int cCount=0;
	
	
	
	//checking if the bullets collide with mushroom
	if(gameGrid[bx][by]==3 || gameGrid[bx][by]==2 || (gameGrid[bx][by]==1 && score++)   )
	{
	
		
		gameGrid[bx][by]--; 	
 		bullet[exists]=false;
 		 
	}
	
	//checking if the bullet reaches top of window
	else if(bullet[y] < -16 )
	{
	 bullet[exists]=false; //if so then delete the bullet
	
	}
	else gameGrid[bx][by]=9;
	
	////////////////////////////////////////////////////////////
	for(int i=0;i<12; i++)
	{
		
		if(bullet[x]/32==centipede[i][x] && bullet[y]/32==centipede[i][y])
		{
			if(bullet[x]/32==centipede[0][x] && bullet[y]/32==centipede[0][y])
			{
				for(int i=0; i<12; i++)
				{ 
				centipede[i][exists]=0;
				for(int j=1; j<12; j++)
				{
					if(centipede[j][exists]) cCount++;
				}
				score+=20 + cCount*10;
				}
				bullet[exists]=false;
				gameGrid[bx][by]=3;
				break;
			}
			
			
			bullet[exists]=false;
			centipede[i][exists]=0;
			
			centipede[i][xDir] ? centipede[i-1][ifHead]=1 : centipede[i+1][ifHead]=1;
			
			gameGrid[bx][by]=3;
			//mushroom[bx][by]=6;
		}
		
	}
	
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);	
	window.draw(bulletSprite);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	 										//sf::Clock bodyClock[4]
void wave(int centipede[][6],int head[],sf::Clock& centiClock)
{


if (centiClock.getElapsedTime().asMilliseconds() < 60)
	return;
centiClock.restart();
for(int i=0; i<12; i++)
{
	if(centipede[i][xDir]==0) //the drection is left ward
	{
		
		if(gameGrid[centipede[i][x]-1][centipede[i][y]]==3 || centipede[i][x]==0)
		{
			centipede[i][xDir]=1;
			if (centipede[i][y]<29)
				centipede[i][yDir]==0 ?  centipede[i][y]++ : centipede[i][y]--;  //moving downward or upward
			else
			{ 
				centipede[i][yDir] = !centipede[i][yDir]; centipede[i][yDir]==0 ?  centipede[i][y]++ : centipede[i][y]--;
			}	
			
			if (centipede[i][yDir] && centipede[i][y]==24) 
			{
				centipede[i][yDir] = !centipede[i][yDir]; centipede[i][yDir]==0 ?  centipede[i][y]++ : centipede[i][y]--;
			}
		}
		else 
		{
			centipede[i][x]--;
		} 
			
	}
	else if(centipede[i][xDir]==1) //the drection is right ward
	{
		if (gameGrid[centipede[i][x]+1][centipede[i][y]]==3 || centipede[i][x]>=28 ) 
		{
			if (centipede[i][y]<29) centipede[i][yDir]==0 ?  centipede[i][y]++ : centipede[i][y]--;  //moving downward or upward
			else
			{
				centipede[i][yDir] = !centipede[i][yDir]; centipede[i][yDir]==0 ?  centipede[i][y]++ : centipede[i][y]--;
			}
			if (centipede[i][yDir]&&centipede[i][y]==24) 
			{
				centipede[i][yDir] = !centipede[i][yDir]; centipede[i][yDir]==0 ?  centipede[i][y]++ : centipede[i][y]--;
			}
			centipede[i][xDir]=0;
		                         //reversing the horizontal direction
		}
	else
	{
		centipede[i][x]++;
	} 
		
	}


}
}



///////////////////////////////////////////////////////////////////////////////////
//Drawing head of centipede
void drawCrHead(sf::RenderWindow& window, sf::Sprite& bodySprite, sf::Sprite& centipedeSprite,int head[],int centipede[][6]) {

	for(int i=0; i<12; i++)
	{
		switch (centipede[i][ifHead])
		{
			case 1:
				if(centipede[i][exists])
				{
					centipedeSprite.setPosition(static_cast<float> (centipede[i][x])*32,static_cast<float> (centipede[i][y])*32);
					window.draw(centipedeSprite);
					break;
				}			
			case 0:
				if(centipede[i][exists])
				{
					bodySprite.setPosition(static_cast<float> (centipede[i][x])*32,static_cast<float> (centipede[i][y])*32);
					window.draw(bodySprite);
					break;
				}
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////
//postion of centipede
void sethead(int centipede[][6],int head[],sf::Clock& bodyClock,int size,int nMushrooms)
{

	int xx,yy;
	
	centipede[0][x]=rand()%16; // the x position of centipede
	centipede[0][y]=rand()%18; //row
	centipede[0][exists]=1;
	centipede[0][xDir]=0;//0 is for left and 1 is for right
	centipede[0][yDir]=0; //0 is for moving the centipede downward
	centipede[0][ifHead]=1; //this part is head which will help in printing
	//////////////////////////////
	gameGrid[centipede[0][x]][centipede[0][y]]=6;
	std::cout << "centipede[0][x] = " << centipede[0][0] << std::endl;
    std::cout << "centipede[0][y] = " << centipede[0][1] << std::endl;
    std::cout << "centipede[0][exists] = " << centipede[0][2] << std::endl;
    std::cout << "centipede[0][xDir] = " << centipede[0][3] << std::endl;
    std::cout << "centipede[0][yDir] = " << centipede[0][4] << std::endl;
    std::cout << "centipede[0][ifHead] = " << centipede[0][5] << std::endl;
	for(int i =1; i<12; i++) //initializing the body parts in the array 
	{
		centipede[i][x]=centipede[i-1][x]+1; // the x position of centipede
		centipede[i][y]=centipede[i-1][y]; //rand()%29; //row
		centipede[i][exists]=1;
		centipede[i][xDir]=0;//0 is for left and 1 is for right
		centipede[i][yDir]=0; //0 is for moving the centipede downward
		centipede[i][ifHead]=0;
		
	xx=centipede[i][x];
	yy=centipede[i][y];
	gameGrid[xx][yy]=6;
	
	}

	}
















