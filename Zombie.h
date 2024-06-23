#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:
	//How fast is each type of zombie?
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//how tough each type of zombie is
	const float BLOATER_HEALTH = 5;
	const float CHASTER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//Make each zombie vary its speed slightly
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	//Where is this zombie?
	Vector2f m_Position;

	//A sprite for the zombie
	Sprite m_Sprite;

	//How fast can this zombie move
	float m_Speed;

	//how much health has it got
	float m_Health;

	//Is it still alive?
	bool m_Alive;

public:

	//when bullet hits zombie
	bool hit();

	//Find out if the zombie is alive
	bool isAlive();

	//Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);

	//Return a rectangle that is the position in the world
	FloatRect getPosition();

	//Get a copy of the sprite to draw
	Sprite getSprite();

	//Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);

};