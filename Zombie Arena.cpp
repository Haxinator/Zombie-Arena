// Zombie Arena.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"


int main()
{
	//instance of texture holder.
	TextureHolder holder;

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
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	//Prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//100 bullets
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed.
	Time lastPressed;

	//hide mouse pointer and replace with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");

	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//About the game
	int score = 0;
	int hiScore = 0;

	//for home and game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	//scale game over screen to fit screen
	Vector2u textureSize = textureGameOver.getSize();
	spriteGameOver.setScale(resolution.x/textureSize.x, resolution.y/textureSize.y);

	//create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(resolution.x * 20/1920, resolution.y * 980/1080);

	//load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(resolution.y * 155/1080);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(resolution.x * 400/1920, resolution.y * 400/1080);
	pausedText.setString("Press Enter \nto continue");

	//Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(resolution.y * 125/1080);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(resolution.x * 250/1920, resolution.y * 850/1080);
	gameOverText.setString("Press Enter to Play");

	//Leveling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(resolution.y * 80/1080);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(resolution.x * 150 / 1920, resolution.y * 250 / 1080);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire\n" <<
		"2- Increased clip size (next reload)\n" <<
		"3- Increased max health\n" <<
		"4- Increased run speed\n" <<
		"5- frequent and better health pickups\n" <<
		"6- frequent and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	//Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(resolution.y * 55/1080);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(resolution.x * 200/1920, resolution.y * 980/1080);

	//Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(resolution.y * 55/1080);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(resolution.x * 20/1920, 0);

	//load highscore from text file data
	std::ifstream inputfile("gamedata/scores.txt");
	if (inputfile.is_open())
	{
		//>> reads data
		inputfile >> hiScore;
		inputfile.close();
	}


	//Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(resolution.y * 55/1080);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(resolution.x * 1400/1920, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	//Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(resolution.y * 55/1080);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(resolution.x * 1500/1920, resolution.y * 980/1080);
	zombiesRemainingText.setString("Zombies: 100");

	//Wave Number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(resolution.y * 55/1080);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(resolution.x * 1250/1920, resolution.y * 980/1080);
	waveNumberText.setString("Wave: 0");

	//Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(resolution.x * 450/1920, resolution.y * 980/1080);

	//When did we last update our hud?
	int framesSinceLastHUDUpdate = 0;
	//how often we should update the hud (in frames)
	int fpsMeasurementFrameInterval = 1000;

	//Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	//Prepare splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	//Prepare shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	//Prepare reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	//Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	//Prepare powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	//Prepare pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

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
					wave = 0;
					score = 0;

					//prepare gun and ammo for next game
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					//Reset players stats
					player.resetPlayerStats();
				}

				if (state == State::PLAYING)
				{
					//Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							//plenty of bullets reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0)
						{
							//Only a few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else
						{
							reloadFailed.play();
						}
					}
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

			//Fire a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if ((gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds())
					> (1000 / fireRate) && (bulletsInClip > 0))
				{
					//Pass the center of the player
					//and center of the cross-hair to the shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;

					if (currentBullet > 99)
					{
						currentBullet = 0;
					}

					lastPressed = gameTimeTotal;

					shoot.play();

					bulletsInClip--;
				}
			} //END fire bullet.

		} //End WASD Handling while playing

		//Handle the LEVELING up state
		if (state == State::LEVELING_UP)
		{
			//Handle the player leveling up
			if (event.key.code == Keyboard::Num1)
			{
				//increase firerate
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				//increase clip size
				clipSize += clipSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				//increase health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				//Increase speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				//Upgrade pickup health
				healthPickup.upgrade();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				//upgrade pickup ammo
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				//increase wave number
				wave++;

				//Prepare the level
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;

				//Pass the vertex array by reference
				//to the createBackground function
				int tileSize = createBackground(background, arena);

				//Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				//configure pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//Create a horde of zombies
				numZombies = 5 * wave;

				//Delete previously allocated memory (if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				//play the powerup sound
				powerup.play();

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

			//Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			//Update player
			player.update(dtSeconds, Mouse::getPosition());

			//make a note of the player's new position
			Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());

			//Loop through each zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtSeconds);
				}
			}

			//Update the pickups
			healthPickup.update(dtSeconds);
			ammoPickup.update(dtSeconds);

			//Collision Detection
			//Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							//Stop the bullet
							bullets[i].stop();

							//Register the hit and see if it was killed
							if (zombies[j].hit())
							{
								//not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numZombiesAlive--;

								//When all the zombies are dead (again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}

								//make splat sound
								splat.play();
							}
						}
					}
				}
			} // End zombie being shot

			//Have any zombies touched the player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						//<< writes data
						outputFile << hiScore;
						outputFile.close();
					}
				}
			} // End player touched

			//Has the player touched health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				//play sound
				pickup.play();
			}

			//Has the player touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
				//play sound
				reload.play();
			}

			//set up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));

			//increment the number of frames since last update
			framesSinceLastHUDUpdate++;

			//re-calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				//Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				//Update score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				//Update hiscore text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				//update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				//Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;

			} // end HUD update

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

			//Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			//draw the bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			//Draw the Player
			window.draw(player.getSprite());

			//Draw the pickup, if currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//draw the crosshair
			window.draw(spriteCrosshair);

			//Switch to the HUD view
			//Draw the HUD
			window.setView(hudView);

			//draw all the hud elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();

	} //end game loop

	//delete any memory if it exists (should be unnecessary)
	delete[] zombies;

	return 0;
}

