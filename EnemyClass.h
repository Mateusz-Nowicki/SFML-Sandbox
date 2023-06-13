#pragma once
#include "GameObject.h"

class EnemyClass
{
private:
	GameObject* _gameObject = nullptr;
	float _health = 100.0;
	float _baseSpeed = 60;
	Vector2f _directionVector;

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


	EnemyClass(Vector2f spawnPos, Vector2f destination)
	{
		_gameObject = new GameObject("Assets\\player.png", 6, 10, 48, 48, new int [10] {6, 6, 6, 6, 6, 6, 4, 4, 4, 3});
		_directionVector = GetCalculatedDirection(spawnPos, destination);
	}

	Vector2f& GetCalculatedDirection(Vector2f spawnPos, Vector2f destination)
	{
		Vector2f directionVector = (destination - spawnPos);
		directionVector = normalize(directionVector);

		//cout << "DirVec: " << directionVector.x << " " << directionVector.y << endl;
		return directionVector;
	}

	void UpdateMovement(float deltaTime, GameObject* GetGameObject)
	{
		_directionVector = GetCalculatedDirection(_gameObject->getPosition(), GetGameObject->getPosition());
		Vector2f translation = (_directionVector * _baseSpeed * deltaTime);
		Vector2f destination = _gameObject->getPosition() + translation;
		_gameObject->move((_directionVector * _baseSpeed * deltaTime));
	}

	GameObject* GetGameObject()
	{
		return _gameObject;
	}
};