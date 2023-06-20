#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "ShurikenClass.h"
#include <algorithm>

class ShurikenController
{
private:
	
	
public:
	vector<ShurikenObject*> _shurikensContainer;
	ShurikenController()
	{
	}



	void SpawnShuriken(Vector2f spawnPos, Vector2f destination)
	{
		ShurikenObject* shurikenObject = new ShurikenObject(spawnPos, destination);
		shurikenObject->GetGameObject()->setPosition(spawnPos);
		cout << "DebugShurikenPos: Spawned at: " << spawnPos.x << "  " << spawnPos.y << endl;
		_shurikensContainer.push_back(shurikenObject);
		cout << "Current shurikencontainer size: " << _shurikensContainer.size() << endl;
	}


	void DrawShuriken(RenderWindow& renderTarget)
	{
		for (ShurikenObject* shurikenObject : _shurikensContainer)
		{
			renderTarget.draw(*shurikenObject->GetGameObject());
		}
	}

	void Update(float deltaTime)
	{
		for (ShurikenObject* shurikenObject : _shurikensContainer)
		{
			shurikenObject->Update(deltaTime);
		}
	}

	vector<ShurikenObject*> GetContainer()
	{
		return _shurikensContainer;
	}
};