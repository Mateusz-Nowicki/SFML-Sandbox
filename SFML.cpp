// SFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//




/// Ogólna klasa: Character.
// W tej klasie bêdzie: 
// Chodzenie. 
/*
    character.Walk();

    void Walk()
    {
        // Wykrycie inputu z klawiatury?
        // Bêdzie trzeba przesun¹æ sprita. Easy - characterSprite.move(x, y);
        // CharacterSpeed? FPSy nie powinny wp³ywaæ na prêdkoœæ ruchu postaci.
        // Trzeba dodaæ Time.deltaTime.
        // characterSprite.Draw();
    }
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include <cstdlib>
#include <iomanip>
#include "PlayerController.h"
#include "PlayerActions.h"
#include <cmath>
#include "GameMode.h"
#include <vector>
#include "ShurikenController.h"
using namespace sf;
using namespace std;


//void SpawnShuriken(vector<GameObject*>& ShurikensContainer, Vector2f StartingPos)
//{
//    GameObject* Shuriken = new GameObject("Assets\\Shuriken.png", 1, 1, 16, 16, new int [1] {1});
//    Shuriken->setPosition(StartingPos);
//    ShurikensContainer.push_back(Shuriken);
//}

void PhysicsLoop();
void Update(float);
void LogicLoop(float deltaTime);
void RenderingLoop(RenderWindow&, float);
void Initialization();
Vector2f CalculateDirection(GameObject* GameObject1, GameObject* GameObject2, double Friction, IntRect intersection);



Clock Mainclock;
Time deltaTime;

GameObject* MouseCursor;
vector<GameObject*> _gameObjectsContainer;
//GameObject* tileMap;
GameObject*** tileMapArray;

int InitialEnemiesCount = 10;

int TileMapWidth = 40;
int TileMapHeight = 30;
int WindowHeight = 480;
int WindowWidth = 640;
bool isProgramFocused = true;

GameMode CurrentGameMode = GameMode::Editor;
ShurikenController* shurikenController;

PlayerController* playerController;
View mainView;
Vector2i mousePosI;
Vector2f mousePosF;
Vector2f windowQuarter;

int minTileIndex = 0;
int maxTileIndex = 47;
int currentTileIndex = 0;

int activeTileX, activeTileY;



int TileMapSet[30][40] =
{
   { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   { 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9},
   {13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15},
};



void EditorModeInput(Event event)
{
    if (event.type == event.MouseWheelMoved)
    {
        if (event.mouseWheel.delta > 0)
        {
            currentTileIndex++;
            if (currentTileIndex > maxTileIndex)
            {
                currentTileIndex = minTileIndex;
            }
        }
        if (event.mouseWheel.delta < 0)
        {
            currentTileIndex--;
            if (currentTileIndex < minTileIndex)
            {
                currentTileIndex = maxTileIndex;
            }
        }
    }
    else if (event.type == event.MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            TileMapSet[activeTileY][activeTileX] = currentTileIndex;
        }
    }
}


void NonEditorModeInput(Event event)
{
    if (event.type == event.MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
           shurikenController->SpawnShuriken(playerController->gameObject()->GetCenteredPosition(), MouseCursor->getPosition());
        }
    }
}

int main()
{
    srand(time(NULL));

    // TODO: Load settings from file (resolution, vsync, target fps, window position, window mode, etc.)
    RenderWindow mainWindow(VideoMode(WindowWidth, WindowHeight), "SFML-Sandbox");

    mainWindow.setFramerateLimit(144);
    mainWindow.setMouseCursorVisible(false);
    Initialization();
    mainWindow.setView(mainView);

    while (mainWindow.isOpen())
    {
        deltaTime = Mainclock.restart();
        //cout << "Delta time: " << deltaTime.asSeconds() << endl;
        // Check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (mainWindow.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                mainWindow.close();
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                isProgramFocused = true;
            }
            else if (event.type == sf::Event::LostFocus)
            {
                isProgramFocused = false;
            }
            else if (event.type == event.KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Tab)
                {
                    switch (CurrentGameMode)
                    {
                        case GameMode::Editor:
                        {
                            CurrentGameMode = GameMode::NonEditor;
                            break;
                        }
                        case GameMode::NonEditor:
                        {
                            CurrentGameMode = GameMode::Editor;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                           
                    }
                }
            }
            else
            {
                switch (CurrentGameMode)
                {
                case GameMode::Editor:
                {
                    EditorModeInput(event);
                    break;
                }
                case GameMode::NonEditor:
                {
                    NonEditorModeInput(event);
                    break;
                }
                default:
                {
                    break;
                }

                }
            }
        }


        if (!isProgramFocused) continue;
        mousePosI = Mouse::getPosition(mainWindow);
        mousePosF.x = mousePosI.x;
        mousePosF.y = mousePosI.y;

        MouseCursor->setPosition(mousePosF * 0.5f + mainView.getCenter() - windowQuarter);

        PhysicsLoop();
        Update(deltaTime.asSeconds());
        LogicLoop(deltaTime.asSeconds());
        RenderingLoop(mainWindow, deltaTime.asSeconds());
    }

    return 0;
}

void Initialization()
{
    
    windowQuarter = Vector2f(WindowWidth, WindowHeight) * 0.25f;
    // load a 32x32 rectangle that starts at (10, 10)
    
    //gameObject = new GameObject*[ArraySize];



    for (int i = 0; i < InitialEnemiesCount; i++)
    {
        GameObject* gameObject = new GameObject("Assets\\player.png", 6, 10, 48, 48, new int [10] {6, 6, 6, 6, 6, 6, 4, 4, 4, 3});
        _gameObjectsContainer.push_back(gameObject);
    }

    //tileMap = new GameObject("Assets\\tile-map-1.png", 6, 8, 16, 16, new int[0] {}, true);

    tileMapArray = new GameObject**[TileMapWidth];
    for (int x = 0; x < TileMapWidth; x++)
    {
        tileMapArray[x] = new GameObject*[TileMapHeight];
        for (int y = 0; y < TileMapHeight; y++)
        {
           // tileMap->SetAnimation(6);
            tileMapArray[x][y] = new GameObject("Assets\\tile-map-1.png", 6, 8, 16, 16, new int[0] {}, true);
        }
    }
    MouseCursor = new GameObject("Assets\\cursor-nc.png", 1, 1, 512, 512, new int[1] {1});

    Vector2f Factors = Vector2f(0.0650,0.0650);
    MouseCursor->SetGlobalScale(Factors);
  

    int minX = 0, maxX = 640;
    int minY = 0, maxY = 480;

    for (GameObject* gameObject : _gameObjectsContainer)
    {
        gameObject->move(Vector2f(((minX)+rand() % (maxX - minX + 1 - 48)), ((minY)+rand() % (maxY - minY + 1 - 48))));
        gameObject->SetAnimation(rand() % 10);
    }
    
    
    GameObject* playerObject = new GameObject("Assets\\player.png", 6, 10, 48, 48, new int [10] {6, 6, 6, 6, 6, 6, 4, 4, 4, 3});
    playerController = new PlayerController(
        playerObject,
        map<PlayerActions, Keyboard::Key>
        {
            { PlayerActions::Left, Keyboard::A},
            { PlayerActions::Right, Keyboard::D },
            { PlayerActions::Up, Keyboard::W },
            { PlayerActions::Down, Keyboard::S }
        },
        250.f
    );
    shurikenController = new ShurikenController();

    mainView = View(playerController->gameObject()->GetCenteredPosition(), Vector2f(WindowWidth / 2, WindowHeight / 2));
}



void PhysicsLoop() 
{
    IntRect intersection;
    double Friction = 0.5;


    for (GameObject* gameObject : _gameObjectsContainer)
    {
        shurikenController->CheckCollisions(gameObject, intersection);
        if (playerController->Collides(*gameObject, intersection))
        {
            Vector2f pushDirection = CalculateDirection(gameObject, playerController->gameObject(), Friction, intersection);
            gameObject->move(pushDirection);
        }
    }



    for (int i = 1; i < _gameObjectsContainer.size(); i++)
    {
        for (int j = 1 + i; j < _gameObjectsContainer.size(); j++)
        {
            if (_gameObjectsContainer[j]->Collides(*_gameObjectsContainer[i], intersection))
            {

                Vector2f pushDirection = CalculateDirection(_gameObjectsContainer[i], _gameObjectsContainer[j], Friction, intersection);
               
                _gameObjectsContainer[i]->move(pushDirection * 0.5f);
                _gameObjectsContainer[j]->move(pushDirection * -0.5f);
            }
        }  
    }

    // TODO: It shouldn't work in this order of operations, we change it again in RenderLoop, but change is persistent. IDK WHY?!



    if (CurrentGameMode == GameMode::Editor)
    {
        for (int y = 0; y < TileMapHeight; y++)
        {
            for (int x = 0; x < TileMapWidth; x++)
            {
                if (tileMapArray[x][y]->Collides(*MouseCursor, intersection))
                {
                    activeTileX = x;
                    activeTileY = y;
                    tileMapArray[x][y]->SetAnimation(currentTileIndex);
                    x = TileMapWidth;
                    y = TileMapHeight;
                }
            }
        }
    }
   
}  


Vector2f CalculateDirection(GameObject* GameObject1, GameObject* GameObject2, double Friction, IntRect intersection)
{
    Vector2f DirectionVector = GameObject1->getPosition() - GameObject2->getPosition();
    double DirectionAngleV = atan2(DirectionVector.y, DirectionVector.x);

    double cosVal = cos(DirectionAngleV);
    double sinVal = sin(DirectionAngleV);

    if (cosVal < Friction && cosVal > -Friction) cosVal = 0;
    if (sinVal < Friction && sinVal > -Friction) sinVal = 0;

    double pushX = cosVal * (intersection.width);
    double pushY = sinVal * (intersection.height);
    return Vector2f(pushX, pushY);
}

void Update(float deltaTime)
{
    
    playerController->Update(deltaTime);
    shurikenController->Update(deltaTime);
    //cout << "DebugMouse: Moved from " << MouseCursor->getPosition().x << " " << MouseCursor->getPosition().y << endl;
    
}
void LogicLoop(float deltaTime) 
{
    playerController->gameObject()->Update(deltaTime);
    
   // tileMap->Update(deltaTime);
    for (int y = 0; y < TileMapHeight; y++)
    {
        for (int x = 0; x < TileMapWidth; x++)
        {
            tileMapArray[x][y]->Update(deltaTime);
        }
    }

    for (GameObject* gameObject : _gameObjectsContainer)
    {
        gameObject->Update(deltaTime);
    }

    
}

void UpdateMainView(RenderWindow& mainWindow)
{
    Vector2f newViewCenter = playerController->gameObject()->GetCenteredPosition();

    if (newViewCenter.x < WindowWidth / 4)
    {
        newViewCenter.x = WindowWidth / 4;
    }
    if (newViewCenter.y < WindowHeight / 4)
    {                           
        newViewCenter.y = WindowHeight / 4;
    }
    if (newViewCenter.x > WindowWidth * 3/4)
    {
        newViewCenter.x = WindowWidth *  3/4;
    }
    if (newViewCenter.y > WindowHeight * 3 / 4)
    {
        newViewCenter.y = WindowHeight * 3 / 4;
    }

    mainView.setCenter(newViewCenter);
    mainWindow.setView(mainView);
}

void RenderingLoop(RenderWindow& renderWindow, float deltaTime)
{
    UpdateMainView(renderWindow);
    renderWindow.clear(Color::Black);
    
    //renderWindow.draw(*tileMap);
    

   for (int y = 0; y < TileMapHeight; y++)
   {
       for (int x = 0; x < TileMapWidth; x++)
       {
         
           tileMapArray[x][y]->SetAnimation(TileMapSet[y][x]);
           tileMapArray[x][y]->setPosition(x * 16, y * 16);
           renderWindow.draw(*tileMapArray[x][y]);
           
       }
   }

   for (GameObject* gameObject : _gameObjectsContainer)
    {
        renderWindow.draw(*gameObject);
    }

    renderWindow.draw(*MouseCursor);
    renderWindow.draw(*playerController->gameObject());

#ifdef DEBUG_DRAW   
    sf::Vertex line[] =
    {
        Vertex(playerController->gameObject()->GetCenteredPosition()),
        Vertex(MouseCursor->GetCenteredPosition()),
    };

    renderWindow.draw(line, 2, sf::Lines);
#endif
 //6x10
    shurikenController->DrawShuriken(renderWindow);

    renderWindow.display();
}





// func load sprite atlas ma nie byæ global, co tam sie dzieje ma wypierdalac do œrodka jako priv metoda klasy gameobjkect, 
// construktor ma zostac zmieniony z przyjmowania spritearray, ma przyjmowaæ stepy, sizy oraz string path to spriteatlas, sprite ma wypierdalaæ z globala, i ma sobie za³adowaæ wszystko 
// drugi objekt i jeden ma sie ruszac a drugi ma stac
