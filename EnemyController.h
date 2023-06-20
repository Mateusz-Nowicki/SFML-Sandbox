#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "EnemyClass.h"
#include <algorithm>

class EnemyController
{
private:

	vector<EnemyClass*> _EnemiesContainer;
	int _initialEnemiesCount;


public:
	int DeadEnemies = 0;

	Vector2f GetRandomWindowPos()
	{
		int minX = 0, maxX = 640;
		int minY = 0, maxY = 480;

		return Vector2f(((minX)+rand() % (maxX - minX + 1 - 48)), ((minY)+rand() % (maxY - minY + 1 - 48)));
	}

	EnemyController(int initialEnemiesCount, Vector2f startingDestinationPos)
	{
		this->_initialEnemiesCount = initialEnemiesCount;

		for (int i = 0; i < _initialEnemiesCount; i++)
		{
			EnemyClass* enemyObject = new EnemyClass(GetRandomWindowPos(), startingDestinationPos);
			_EnemiesContainer.push_back(enemyObject);
			enemyObject->GetGameObject()->setPosition(startingDestinationPos);
			enemyObject->GetGameObject()->SetAnimation(rand() % 10);
		}
	}

	void SpawnNewEnemies(float difficulty, Vector2f destination)
	{
		for (int x = 0; x < DeadEnemies; x++)
		{
			for (int i = 0; i < (int)difficulty; i++)
			{
				SpawnEnemy(GetRandomWindowPos(), destination);
			}
		}
		DeadEnemies = 0;
	}

	void SpawnEnemy(Vector2f spawnPos, Vector2f destination)
	{
		EnemyClass* enemyClass = new EnemyClass(spawnPos, destination);
		enemyClass->GetGameObject()->setPosition(spawnPos);
		_EnemiesContainer.push_back(enemyClass);
	}


	void DrawEnemy(RenderWindow& renderTarget)
	{
		for (EnemyClass* EnemyClass : _EnemiesContainer)
		{
			renderTarget.draw(*EnemyClass->GetGameObject());
		}
	}



	void Update(float deltaTime, Vector2f objectPos)
	{
		for (EnemyClass* enemyClass : _EnemiesContainer)
		{
			enemyClass->Update(deltaTime, objectPos);
			enemyClass->GetGameObject()->Update(deltaTime);
		}

	}

	vector<EnemyClass*> GetContainer()
	{
		return _EnemiesContainer;
	}

	int GetDeadEnemies()
	{
		return DeadEnemies;
	}
};