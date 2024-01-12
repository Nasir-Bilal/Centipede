//last edit: 10:42 23 november

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void movSprite(sf::Clock& bulletClock,float player[], int&,int&); //1.moving the sprite
void shot(float bullet[],float player[]); //2.shooting lazer bullets
void drawMushroom(sf::RenderWindow& window, float mushroom[][gameColumns], sf::Sprite& mushroomSprite,int& mBreakx,int& mBreaky); //3.drawing mushroom
void setMushroom( float mushroom[][gameColumns]);//4.initializing the position of mushrooms 
void drawCrHead(sf::RenderWindow& window, int startP, sf::Sprite& crHeadSprite);
void drawCrBody(sf::RenderWindow& window,sf::Clock& crBodyClock, int startP, sf::Sprite& crBodySprite);

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock,int&,int&,int&,int&);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);

void printGameGrid();

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
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	//bgMusic.play();
	bgMusic.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.4)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2)*boxPixelsX;
	player[y] = (gameColumns * 3 / 4)* boxPixelsY;
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
	setMushroom(mushroom);//calling function to set the postion of  Mushrooms 
	int mBreakx=0,mBreaky=0;
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(mBreakx , mBreaky , boxPixelsX, boxPixelsY));
	
	
	int px,py; //declaring variables to compare postion of mushroom on game grid
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//centiped's right head declaration
	float crHead[gameRows][gameColumns];
	const int startP=12+rand()%6;
	gameGrid[startP][0]=6;
	sf::Clock crHeadClock;
	sf::Texture crHeadTexture;
	sf::Sprite crHeadSprite;
	crHeadTexture.loadFromFile("Textures/crHead.png");
	crHeadSprite.setTexture(crHeadTexture);
	int AnimationHeadX,AnimationHeadY;//Setting texture of Head to start. It'll help in animation
	AnimationHeadX=AnimationHeadY=0;
	crHeadSprite.setTextureRect(sf::IntRect(AnimationHeadX , AnimationHeadY , boxPixelsX, boxPixelsY));
	
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//centiped's right head declaration
	float crBody[gameRows][gameColumns];
	
	for(int i=0; i<gameColumns; i++)
	{	
		if(gameGrid[i][0]==6)
		{
		 i++;
		 int length=i+10;
			for(i; i< length;i++)
			{ 
				
				gameGrid[i][0]=7;
					
			}
			 break;
		}
		
	
	}
	
	sf::Clock crBodyClock;
	sf::Texture crBodyTexture;
	sf::Sprite crBodySprite;
	crBodyTexture.loadFromFile("Textures/crBody.png");
	crBodySprite.setTexture(crBodyTexture);\
	int AnimationBodyX,AnimationBodyY;//Setting texture of body to start. It'll help in animation
	AnimationBodyX=AnimationBodyY=0;
	crBodySprite.setTextureRect(sf::IntRect(AnimationBodyX , AnimationBodyY , boxPixelsX, boxPixelsY));
	
	//////////////////////////////////////////////////////////////////////////
	
	
	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////

		
		//Calling the function to move the sprite and shot bullet
		movSprite(playerClock,player,px,py);
		shot(bullet,player);
		
		
		cout<<endl<<"bullet x: "<<bx<<endl
		<<endl<<"bullet y: "<<by<<endl;
		
		 

		
		
		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);
		drawCrHead(window, startP, crHeadSprite);
		drawCrBody(window,crBodyClock ,startP, crBodySprite);
		drawMushroom(window, mushroom, mushroomSprite,mBreakx,mBreaky);
		//dataMushroom();
		
		printGameGrid(); //the function to print the array of game grid
		
		if (bullet[exists] == true) {
			moveBullet(bullet, bulletClock,bx,by,mBreakx,mBreaky);
			drawBullet(window, bullet, bulletSprite);
		}
		
		
		
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			cout<<"mbreakx: "<<mBreaky;
				return 0;
			}
		}		
		window.display();
		window.clear();
		
	}
	
	
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
//Initializing postion of  mushrooms
void setMushroom( float mushroom[][gameColumns]) 
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
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
//drawing mushroom
void drawMushroom(sf::RenderWindow& window, float mushroom[][gameColumns], sf::Sprite& mushroomSprite,int& mBreakx,int& mBreaky) 
{
	
	
	for(int i=0; i < gameRows; i++)
		{
			for(int j=0; j < gameColumns; j++)
			{
				if(mushroom[i][j]==6 ) 
				{	
					
					if(gameGrid[i][j]==3)
					{
						mushroomSprite.setPosition(i*32, j*32);
						mushroomSprite.setTextureRect(sf::IntRect(0 , 32 , boxPixelsX, boxPixelsY));
						window.draw(mushroomSprite);
					}
					if(gameGrid[i][j]==2)
					{
						mushroomSprite.setPosition(i*32, j*32);
						mushroomSprite.setTextureRect(sf::IntRect(32 , 32 , boxPixelsX, boxPixelsY));
						window.draw(mushroomSprite);
					}
					if(gameGrid[i][j]==1)
					{
						mushroomSprite.setPosition(i*32, j*32);
						mushroomSprite.setTextureRect(sf::IntRect(64 , 32 , boxPixelsX, boxPixelsY));
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !bullet[exists]) {
           bullet[x] = player[x];
            bullet[y] = player[y] - boxPixelsY; //first sets all the value of bullet coordinates above the sprite
            bullet[exists] = true; //turn the existance true if space is pressed
            
        }


}
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//Function that movees the sprite
void movSprite(sf::Clock& playerClock,float player[],int& px, int& py){ 
					//passing px and py by reference

	px=player[x]/32;
	py=player[y]/32;
	
	
	
		//////////////////////////////////////////////////////
		//obsering the pattern of Game grid
		
		cout<<endl<<"player x: "<<player[x]
		    <<endl<<"player y: "<<player[y]<<endl
		 
		    <<"px: "<<px<<endl<<"py: "<<py<<endl
		    <<"value of mushroom at game grid: "
		    <<gameGrid[px][py]<<endl;	
		//printGameGrid();
		///////////////////////////////////////////////////////
	if (playerClock.getElapsedTime().asMilliseconds() < 60) return;
	playerClock.restart();
	
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)  &&  player[x] > 0 && gameGrid[px-1][py]!=3)
{	

	
player[x]-=boxPixelsX;
	
}		
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] < resolutionX-boxPixelsX && gameGrid[px+1][py]!=3) 
	player[x]+=boxPixelsX;
	
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&  player[y] > 0 && gameGrid[px][py-1]!=3)
	 player[y]-=boxPixelsY;
	
if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&  player[y] < resolutionY-boxPixelsY && gameGrid[px][py+1]!=3)
	player[y]+=boxPixelsX;
	
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
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void moveBullet(float bullet[], sf::Clock& bulletClock,int& bx,int& by,int& mBreakx,int& mBreaky) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 20)
		return;
	

	bulletClock.restart();
	bullet[y] -= boxPixelsY;	
	bx=bullet[x]/32;
	by=bullet[y]/32;
	
	
	//checking if the bullets collide with mushroom
	if(gameGrid[bx][by]==3||gameGrid[bx][by]==2||gameGrid[bx][by]==1)
	{
	//if so then increase mBreak(texture of mushroom)
		
		gameGrid[bx][by]--; 	
 		bullet[exists]=false;
 		 
	}
	
	//checking if the bullet reaches top of window
	else if(bullet[y] < -16 )
	{
	 bullet[exists]=false; //if so then delete the bullet
	
	}
	else gameGrid[bx][by]=9;
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);	
	window.draw(bulletSprite);
}

///////////////////////////////////////////////////////////////////////////////////
//Drawing head of centipede
void drawCrHead(sf::RenderWindow& window, int startP, sf::Sprite& crHeadSprite) {

	for(int i=0; i<30; i++){		
		if(gameGrid[i][0]==6)
		{	
		crHeadSprite.setPosition(i*boxPixelsX, 0);	
		window.draw(crHeadSprite);
		}
	}
}


void drawCrBody(sf::RenderWindow& window,sf::Clock& crBodyClock, int startP, sf::Sprite& crBodySprite) {
	
	

	for(int i=0; i<30; i++)
	{		
		
		if(gameGrid[i][0]==7)
		{
		crBodySprite.setPosition(i*boxPixelsX, 0);	
		window.draw(crBodySprite);
		}
	}
}



	
