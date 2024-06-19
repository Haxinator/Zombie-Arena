#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	//position of player
	Vector2f m_Position;

	//sprite
	Sprite m_Sprite;

	//add texture
	Texture m_Texture;

	//What is the screen resolution
	Vector2f m_Resolution;

	//What is the size of the current arena
	IntRect m_Arena;

	//How big is each tile of the arena
	int m_TileSize;

	//What direction is the player moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	//How much health the player has
	int m_Health;

	//What is the maximum health the player can have
	int m_MaxHealth;

	//When was the player last hit
	Time m_LastHit;

	//Speed in pixels per second
	float m_Speed;

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	//call this at the end of every game
	void resetPlayerStats();

	//Hanlde the player getting hit by a zombie
	bool hit(Time timeHit);

	//How long ago was the player last hit
	Time getLastHitTime();

	//Where is the player
	FloatRect getPosition();

	//Where is the center of the player
	Vector2f getCenter();

	//What angle is the player facing
	float getRotation();

	//send a copy of the sprite to the main function
	Sprite getSprite();

	//The next four functions move the player
	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	//stop the player moving in a specific direction
	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	//Call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition);

	//give player speed boost
	void upgradeSpeed();

	//give player some health
	void upgradeHealth();

	//increase maximum health player can have
	void increaseHealthLevel(int amount);

	//How much health has the player currently got?
	int getHealth();
};