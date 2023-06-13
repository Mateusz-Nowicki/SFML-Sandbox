#pragma once
#include "GameObject.h"


class ShurikenObject
{
private:
	float _lifeSpan = 5.0;
	float _baseSpeed = 100;
	Vector2f _directionVector;
	GameObject* _gameObject = nullptr;

	Vector2f normalize(const Vector2f& source)
	{
		float length = sqrt((source.x * source.x) + (source.y * source.y));
		if (length != 0)
		{
			return Vector2f(source.x / length, source.y / length);
		}
		else
		{
			return source;
		}
	}

public:

	ShurikenObject(Vector2f spawnPos, Vector2f destination)
	{
		_gameObject = new GameObject("Assets\\Shuriken.png", 1, 1, 16, 16, new int [1] {1});
		_directionVector = CalculateDirection(spawnPos, destination);
	}

	Vector2f& CalculateDirection(Vector2f spawnPos, Vector2f destination)
	{
		Vector2f directionVector = (destination - spawnPos);
		directionVector = normalize(directionVector);

		//cout << "DirVec: " << directionVector.x << " " << directionVector.y << endl;
		return directionVector;
	}


	void InvertDirection()
	{
		_directionVector = _directionVector * (-1.0f);
	}



	void Update(float deltaTime)
	{
		Vector2f translation = (_directionVector * _baseSpeed * deltaTime);
		Vector2f destination = _gameObject->getPosition() + translation;
		//cout <<
		//	"Moving from: [" << _gameObject->getPosition().x << " " << _gameObject->getPosition().y <<
		//	"] by [" << translation.x << " " << translation.y <<
		//	"] to: [" << destination.x << " " << destination.y <<
		//	"]" << endl;
		_gameObject->move((_directionVector * _baseSpeed * deltaTime));
	}

	GameObject* GetGameObject()
	{
		return _gameObject;
	}

	float GetLifeSpan()
	{
		return _lifeSpan;
	}

	float GetBaseSpeed()
	{
		return _baseSpeed;
	}

};