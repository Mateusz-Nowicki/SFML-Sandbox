#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class GameObject : public sf::Drawable, public sf::Transformable
{
private:
    float _frameTimer = 0;
    float _frameInterval = 1.0f / 6.0f;
    int* _animationLength;
    int _currentAnimationIndex;
    int _currentAnimationFrame;

    int _sizeX;
    int _sizeY;

    int _stepX;
    int _stepY;

    bool _useSingleTile;

    Texture _texture;
    Sprite _sprite;

    IntRect* _boundingBox;

public:
    /* int SizeX = 6;
        int SizeY = 10;
        int StepX = 48;
        int StepY = 48;*/

    

    GameObject(string pathToSpriteAtlas, int sizeX, int sizeY, int stepX, int stepY, int* animationLength, bool useSingleTile = false)
    {
        _useSingleTile = useSingleTile;

        _sizeX = sizeX;
        _sizeY = sizeY;

        _stepX = stepX;
        _stepY = stepY;

        if (!_texture.loadFromFile(pathToSpriteAtlas))
        {
            // error...
        }
        _texture.setSmooth(true);

        if (_useSingleTile)
        {
            animationLength = new int[sizeX * sizeY];
            for (int i = 0; i < sizeX * sizeY; i++)
            {
                animationLength[i] = 1;
            }
        }

        LoadSpriteAtlas(_texture, animationLength);

        _boundingBox = new IntRect(_sprite.getGlobalBounds());
    }

    IntRect GetBoundingBox()
    {
        return IntRect(*_boundingBox);
    }
    void Update(float deltaTime)
    {
        UpdateAnimation(deltaTime);
    }
    
   //void Dispose()
   //{
   //    gameObject = nullptr;
   //    delete gameObject;
   //}

    Vector2f GetCenteredPosition()
    {
        return Vector2f(getPosition().x + _boundingBox->width * 0.5f, getPosition().y + _boundingBox->height * 0.5f);
    }

    void SetAnimation(int animationIndex)
    {
        if (_currentAnimationIndex == animationIndex) return;


        _currentAnimationIndex = animationIndex;
        _currentAnimationFrame = 0;
    }
    bool Collides(GameObject& other, IntRect& intersection)
    {
        IntRect A = IntRect(this->getPosition().x, this->getPosition().y, this->_boundingBox->width, this->_boundingBox->height);
        IntRect B = IntRect(other.getPosition().x, other.getPosition().y, other._boundingBox->width, other._boundingBox->height);
        return A.intersects(B, intersection);
    }

    void SetGlobalScale(Vector2f& Factor)
    {
        this->setScale(Factor);
        this->_boundingBox->height *= Factor.y;
        this->_boundingBox->width *= Factor.x;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        //states.texture = &m_tileset;

        // draw the vertex array
        target.draw(_sprite, states);

        
#ifdef DEBUG_DRAW
        sf::Vertex line[] =
        {
            Vertex(this->getPosition() + Vector2f(0, 0)),
            Vertex(this->getPosition() + Vector2f(_boundingBox->width , 0)),
            Vertex(this->getPosition() + Vector2f(_boundingBox->width , 0)),
            Vertex(this->getPosition() + Vector2f(_boundingBox->width , _boundingBox->height - 1)),
            Vertex(this->getPosition() + Vector2f(_boundingBox->width , _boundingBox->height - 1)),
            Vertex(this->getPosition() + Vector2f(0, _boundingBox->height - 1)),
            Vertex(this->getPosition() + Vector2f(0, _boundingBox->height - 1)),
            Vertex(this->getPosition() + Vector2f(0, 0))
        };
        target.draw(line, 8, sf::Lines);
#endif // DEBUG

        
    }

 

    void LoadSpriteAtlas(Texture& texture, int* animationLenght)
    {

        _sprite = Sprite();
        _sprite.setTexture(texture);
        _sprite.setTextureRect(IntRect(0, 0, _stepX, _stepY));
        _animationLength = animationLenght;
    }

    void UpdateAnimation(float deltaTime)
    {
        if (_useSingleTile)
        {
            int x = _currentAnimationIndex % _sizeX;
            int y = (_currentAnimationIndex - x) / _sizeX;


            _sprite.setTextureRect(IntRect(x * _stepX, y * _stepY, _stepX, _stepY));
            return;
        }



        _frameTimer += deltaTime;

        if (_frameTimer > _frameInterval)
        {
            _currentAnimationFrame++;
            _sprite.setTextureRect(IntRect(_currentAnimationFrame * _stepX, _currentAnimationIndex * _stepY, _stepX, _stepY));

            
            _frameTimer -= _frameInterval;
            if (_currentAnimationFrame > _animationLength[_currentAnimationIndex] - 2)
            {
                _currentAnimationFrame = 0;
            }

        }

    }

    
};



