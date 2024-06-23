// Zombie Arena.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"


int main()
{
	//The game will be in one of four states
	enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

	//start with the game over state
	State state = State::GAME_OVER;

	//get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	VideoMode vm(resolution.x, resolution.y);

	RenderWindow window(vm, "Zombie Arena", Style::Fullscreen);

	//Create a SFML view for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Clock for timing
	Clock clock;

	//How long has PLAYING state been active
	Time gameTimeTotal;

	//Where is the mouse in relation to world coordinates.
	Vector2f mouseWorldPosition;

	//Where is the mouse in relation to screen coordinates.
	Vector2i mouseScreenPosition;

	//Create an instance of the Player class.
	Player player;

	//Boundaries of the arena
	IntRect arena;

	//Create the background
	VertexArray background;
	//Load the texture for our background vertex array
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");

	//main game loop
	while (window.isOpen())
	{
		/*
		Handle Input
		*/

		//Handle events by polling
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				//Pause a game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				//restart while paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;

					//restart the clock so there isn't a frame jump
					clock.restart();
				}

				//start a new game while in game over state
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{

				}
			}
		} // end event polling

		//Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Hanlde WASD while playing
		if (state == State::PLAYING)
		{
			//Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else {
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else {
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else {
				player.stopRight();
			}
		} //End WASD Handling while playing

		//Handle the LEVELING up state
		if (state == State::LEVELING_UP)
		{
			//Handle the player leveling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				//Prepare the level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				//Pass the vertex array by reference
				//to the createBackground function
				int tileSize = createBackground(background, arena);

				//Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				//Reset the clock so there isn't a frame jump
				clock.restart();
			}
		} // END LEVELING UP


		/*
		Update the Frame
		*/
		if (state == State::PLAYING)
		{
			Time dt = clock.restart();

			//update the total game time
			gameTimeTotal += dt;

			//make a decimal fraction of 1 from the delta time
			float dtSeconds = dt.asSeconds();

			//where is mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			//Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//Update player
			player.update(dtSeconds, Mouse::getPosition());

			//make a note of the player's new position
			Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());

		} // END UPDATING SCENE

		/*
		Draw the Scene
		*/
		if (state == State::PLAYING)
		{
			window.clear();

			//Set the mainView to be displayed in the window
			//and draw everything related to it
			window.setView(mainView);

			//Draw the background
			window.draw(background, &textureBackground);

			//Draw the Player
			window.draw(player.getSprite());
		}

		if (state == State::LEVELING_UP)
		{

		}

		if (state == State::PAUSED)
		{

		}

		if (state == State::GAME_OVER)
		{

		}

		window.display();

	} //end game loop

	return 0;
}

