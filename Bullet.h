#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Bullet
{
private:
	//where is the bullet
	Vector2f m_Position;

	// what each bullet looks like
	RectangleShape m_BulletShape;

	//Is this bullet currently whizzing through the air
	bool m_InFlight = false;

	//How fast does a bullet travel?
	float m_BulletSpeed = 1000;

	//what fraction of 1 does the bullet travel each frame
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	//Some boundaries so bullet doesn't fly forever
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	//The constructor
	Bullet();

	//stop the bullet
	void stop();

	//returns the value of m_InFlight
	bool isInFlight();

	//Launch a new bullet
	void shoot(float startX, float startY, float xTarget, float yTarget);

	//Tell the calling code where the bullet is in the world
	FloatRect getPosition();

	//Return the actual shape of the bullet
	RectangleShape getShape();

	//Update bullet each frame
	void update(float elapsedTime);
};