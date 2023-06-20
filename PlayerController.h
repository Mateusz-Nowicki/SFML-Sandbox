#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "PlayerActions.h"

using namespace sf;
using namespace std;

class PlayerController
{
private:
	GameObject* _gameObject = NULL;
    float _baseSpeed;
    float _speedX;
    float _speedY;
    
    map<PlayerActions, Keyboard::Key> _mappedActions;

    Vector2f _motionVector;
    int _stateAnimationIndex = 0;



public:
    const float startingLifePoints = 10;
    float HealthPoints = 10.0;

	PlayerController(GameObject* gameobject, map<PlayerActions, Keyboard::Key> mappedActions, float baseSpeed)
	{
        _mappedActions = mappedActions;
		_gameObject = gameobject;
        _baseSpeed = baseSpeed;
	}

   //void Dispose()
   //{
   //    _gameObject = nullptr;
   //    delete _gameObject;
   //}

    bool Collides(GameObject& other, IntRect& intersection)
    {
        return _gameObject->Collides(other, intersection);
    }

    GameObject* GetGameObject()
    {
        return _gameObject;
    }



    void Update(float deltaTime)
    {
        
        _speedX = _baseSpeed * deltaTime;
        _speedY = _baseSpeed * deltaTime;


        _stateAnimationIndex = 0;

        if (!Keyboard::isKeyPressed(_mappedActions[PlayerActions::Left]) &&
            !Keyboard::isKeyPressed(_mappedActions[PlayerActions::Right]))
        {
            _motionVector.x = 0.0f;
        }

        if (!Keyboard::isKeyPressed(_mappedActions[PlayerActions::Up]) &&
            !Keyboard::isKeyPressed(_mappedActions[PlayerActions::Down]))
        {
            _motionVector.y = 0.0f;
        }

        if (Keyboard::isKeyPressed(_mappedActions[PlayerActions::Left]))
        {
            _stateAnimationIndex = 4;
            _motionVector.x = -_speedX;
            
        }

        if (Keyboard::isKeyPressed(_mappedActions[PlayerActions::Right]))
        {
            _stateAnimationIndex = 4;
            _motionVector.x = _speedX;
            
        }

        if (Keyboard::isKeyPressed(_mappedActions[PlayerActions::Up]))
        {
            _stateAnimationIndex = 5;
            _motionVector.y = -_speedY;
        }

        if (Keyboard::isKeyPressed(_mappedActions[PlayerActions::Down]))
        {
            _stateAnimationIndex = 3;
            _motionVector.y = _speedY;
        }

        _gameObject->SetAnimation(_stateAnimationIndex);
        _gameObject->move(_motionVector);
    }
};