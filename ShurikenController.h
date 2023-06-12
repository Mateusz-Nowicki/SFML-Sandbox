#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "ShurikenClass.h"
#include <algorithm>

class ShurikenController
{
private:

	vector<ShurikenObject*> _shurikensContainer;
	
public:
	ShurikenController()
	{
	}



	void SpawnShuriken(Vector2f spawnPos, Vector2f destination)
	{
		ShurikenObject* shurikenObject = new ShurikenObject(spawnPos, destination);
		shurikenObject->gameObject()->setPosition(spawnPos);
		cout << "DebugShurikenPos: Spawned at: " << spawnPos.x << "  " << spawnPos.y << endl;
		_shurikensContainer.push_back(shurikenObject);
		cout << _shurikensContainer.size() << endl;
	}


	void DrawShuriken(RenderWindow& renderTarget)
	{
		for (ShurikenObject* shurikenObject : _shurikensContainer)
		{
			renderTarget.draw(*shurikenObject->gameObject());
		}
	}

	audhasljdkaljdkaslkjhdaljskdalkjhsd

	void CheckCollisions(GameObject* gameObject, IntRect& intersection)
	{
		for (ShurikenObject* shurikenObject : _shurikensContainer)
		{
			if (gameObject->Collides(*shurikenObject->gameObject(), intersection))
			{
				_shurikensContainer.erase(std::remove(_shurikensContainer.begin(), _shurikensContainer.end(), shurikenObject), _shurikensContainer.end());
				//shurikenObject->gameObject()->Dispose();
				shurikenObject = nullptr;
				delete shurikenObject;
			}
		}
	}


	void Update(float deltaTime)
	{
		for (ShurikenObject* shurikenObject : _shurikensContainer)
		{
			shurikenObject->Update(deltaTime);
		}
	}
};