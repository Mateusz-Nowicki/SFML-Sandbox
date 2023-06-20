

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
#include "EnemyController.h"
#include <sstream>
using namespace sf;
using namespace std;



void UpdateMainView(RenderWindow& mainWindow);
void PhysicsLoop(float);
void Update(float);
void LogicLoop(float deltaTime);
void RenderingLoop(RenderWindow&, float);
void Initialization();
Vector2f CalculateDirection(GameObject* GameObject1, GameObject* GameObject2, double Friction, IntRect intersection);




Clock Mainclock;
Time deltaTime;

GameObject* MouseCursor;
GameObject*** tileMapArray;

int InitialEnemiesCount = 15;
float difficulty = 0.0f;
float difficultyStep = 0.01f;

int TileMapWidth = 40;
int TileMapHeight = 30;
int WindowHeight = 480;
int WindowWidth = 640;
bool isProgramFocused = true;

GameMode CurrentGameMode = GameMode::NonEditor;
ShurikenController* shurikenController;
EnemyController* enemyController;

PlayerController* playerController;
View mainView;
Font algerFont;
Text mainText;
Text welcomeText;
Text startText;
Text currentHP;
Vector2i mousePosI;
Vector2f mousePosF;
Vector2f windowQuarter;

int minTileIndex = 0;
int maxTileIndex = 47;
int currentTileIndex = 0;

int score = 0;
Text scoreText;

int activeTileX, activeTileY;

bool isGameRunning = false;


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

bool Collide(GameObject* Mouse, RectangleShape& Button)
{
    IntRect intersection;
    IntRect A = IntRect(Mouse->getPosition().x, Mouse->getPosition().y, Mouse->GetBoundingBox().width, Mouse->GetBoundingBox().height);
    IntRect B = IntRect(Button.getPosition().x, Button.getPosition().y, Button.getGlobalBounds().width, Button.getGlobalBounds().height);
    return A.intersects(B, intersection);
}

void MenuUI(RenderWindow& renderWindow, Event event)
{
    bool isStartButtonClicked = false;
    IntRect intersection;

    RectangleShape StartButton(Vector2f(128.0f, 32.0f));
    StartButton.setFillColor(Color::Green);
    StartButton.setPosition(mainView.getCenter() - Vector2f(StartButton.getGlobalBounds().width / 2, StartButton.getGlobalBounds().height / 2));
    welcomeText.setFont(algerFont);
    welcomeText.setString("Welcome");
    welcomeText.setCharacterSize(25);
    welcomeText.setFillColor(sf::Color::Red);
    welcomeText.setPosition(mainView.getCenter() - Vector2f(welcomeText.getGlobalBounds().width / 2, (welcomeText.getGlobalBounds().height / 2) + 50));

    startText.setFont(algerFont);
    startText.setString("Start");
    startText.setCharacterSize(25);
    startText.setFillColor(sf::Color::Blue);
    startText.setPosition(mainView.getCenter() - Vector2f(startText.getGlobalBounds().width / 2, (startText.getGlobalBounds().height / 2)));

    
    

   renderWindow.draw(welcomeText);

   if(!isStartButtonClicked)
   {
       if (Collide(MouseCursor, StartButton) && event.type == event.MouseButtonPressed)
       {
           if (event.mouseButton.button == sf::Mouse::Left)
           {
               CurrentGameMode = GameMode::NonEditor;
               isStartButtonClicked = true;
               isGameRunning = true;
               StartButton.setSize(Vector2f(144.f, 48.f));
               StartButton.setPosition(mainView.getCenter() - Vector2f(StartButton.getGlobalBounds().width / 2, StartButton.getGlobalBounds().height / 2));
               
           }
       }
   }
   renderWindow.draw(StartButton);
   renderWindow.draw(startText);
}


void SummaryScreenUI(RenderWindow& renderWindow, Event event)
{
    bool isStartButtonClicked = false;
    IntRect intersection;

    RectangleShape StartButton(Vector2f(128.0f, 32.0f));
    StartButton.setFillColor(Color::Green);
    StartButton.setPosition(mainView.getCenter() - Vector2f(StartButton.getGlobalBounds().width / 2, StartButton.getGlobalBounds().height / 2));
    welcomeText.setFont(algerFont);
    welcomeText.setString("U have died");
    welcomeText.setCharacterSize(25);
    welcomeText.setFillColor(sf::Color::Red);
    welcomeText.setPosition(mainView.getCenter() - Vector2f(welcomeText.getGlobalBounds().width / 2, (welcomeText.getGlobalBounds().height / 2) + 100));

    startText.setFont(algerFont);
    startText.setString("Restart");
    startText.setCharacterSize(25);
    startText.setFillColor(sf::Color::Blue);
    startText.setPosition(mainView.getCenter() - Vector2f(startText.getGlobalBounds().width / 2, (startText.getGlobalBounds().height / 2)));

    scoreText.setFont(algerFont);
    scoreText.setString("Your score is: " + std::to_string(score));
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(sf::Color::Blue);
    scoreText.setPosition(mainView.getCenter() - Vector2f((startText.getGlobalBounds().width / 2) + 50, (startText.getGlobalBounds().height / 2) + 50));

    renderWindow.draw(scoreText);
    renderWindow.draw(welcomeText);

    if (!isStartButtonClicked)
    {
        if (Collide(MouseCursor, StartButton) && event.type == event.MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                CurrentGameMode = GameMode::NonEditor;
                isStartButtonClicked = true;
                isGameRunning = true;
                StartButton.setSize(Vector2f(144.f, 48.f));
                StartButton.setPosition(mainView.getCenter() - Vector2f(StartButton.getGlobalBounds().width / 2, StartButton.getGlobalBounds().height / 2));
                playerController->HealthPoints = playerController->startingLifePoints;
                score = 0;
                difficulty = 0;
                enemyController->DeadEnemies = 0;
            }
        }
    }
    renderWindow.draw(StartButton);
    renderWindow.draw(startText);
}

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
           shurikenController->SpawnShuriken(playerController->GetGameObject()->GetCenteredPosition(), MouseCursor->getPosition());
        }
    }
}

int main()
{
    srand(time(NULL));
    RenderWindow mainWindow(VideoMode(WindowWidth, WindowHeight), "SFML-Sandbox");

    mainWindow.setFramerateLimit(144);
    mainWindow.setMouseCursorVisible(false);
    Initialization();
    mainWindow.setView(mainView);

    while (mainWindow.isOpen())
    {
        deltaTime = Mainclock.restart();
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
        UpdateMainView(mainWindow);
        mainWindow.clear(Color::Black);
        if (isGameRunning)
        {
            PhysicsLoop(deltaTime.asSeconds());
            Update(deltaTime.asSeconds());
            LogicLoop(deltaTime.asSeconds());
            RenderingLoop(mainWindow, deltaTime.asSeconds());
        }
        else
        {
            if (playerController->HealthPoints <= 0)
            {
                SummaryScreenUI(mainWindow, event);
            }
            else
            {
                CurrentGameMode = GameMode::Menu;
                MenuUI(mainWindow, event);
                
            }
        }
        mainWindow.draw(*MouseCursor);
        mainWindow.display();
    }
    
    return 0;
}

void Initialization()
{
    windowQuarter = Vector2f(WindowWidth, WindowHeight) * 0.25f;
    
    if (!algerFont.loadFromFile("Assets\\Fonts\\ALGER.TTF"));
    
    mainText.setFont(algerFont);
    mainText.setString("Mateusz");
    mainText.setCharacterSize(18);
    mainText.setFillColor(sf::Color::Black);

    tileMapArray = new GameObject**[TileMapWidth];
    for (int x = 0; x < TileMapWidth; x++)
    {
        tileMapArray[x] = new GameObject*[TileMapHeight];
        for (int y = 0; y < TileMapHeight; y++)
        {
            tileMapArray[x][y] = new GameObject("Assets\\tile-map-1.png", 6, 8, 16, 16, new int[0] {}, true);
        }
    }

    MouseCursor = new GameObject("Assets\\cursor-nc.png", 1, 1, 512, 512, new int[1] {1});

    Vector2f Factors = Vector2f(0.0650,0.0650);
    MouseCursor->SetGlobalScale(Factors);
  
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
    enemyController = new EnemyController(InitialEnemiesCount, playerController->GetGameObject()->GetCenteredPosition());

    mainView = View(playerController->GetGameObject()->GetCenteredPosition(), Vector2f(WindowWidth / 2, WindowHeight / 2));
}



void PhysicsLoop(float deltaTime) 
{
    IntRect intersection;
    double Friction = 0.5;

    for (int i = 1; i < enemyController->GetContainer().size(); i++)
    {
        for (int j = 1 + i; j < enemyController->GetContainer().size(); j++)
        {
            if (enemyController->GetContainer()[j]->GetGameObject()->Collides(*enemyController->GetContainer()[i]->GetGameObject(), intersection))
            {
                Vector2f pushDirection = CalculateDirection(enemyController->GetContainer()[i]->GetGameObject(), enemyController->GetContainer()[j]->GetGameObject(), Friction, intersection);

                enemyController->GetContainer()[i]->GetGameObject()->move(pushDirection * 0.5f);
                enemyController->GetContainer()[j]->GetGameObject()->move(pushDirection * -0.5f);
            }
        }
    }

  



    for (EnemyClass* enemyObject : enemyController->GetContainer())
    {
        for (ShurikenObject* shurikenObject : shurikenController->GetContainer())
        {
            if (enemyObject->GetGameObject()->Collides(*shurikenObject->GetGameObject(), intersection))
            {
                shurikenObject->ToRemove = true;
                enemyObject->Respawn();
                score++;
                difficulty += difficultyStep;
                enemyController->DeadEnemies++;
            }

        }

        if (playerController->Collides(*enemyObject->GetGameObject(), intersection))
        {
            Vector2f pushDirection = CalculateDirection(enemyObject->GetGameObject(), playerController->GetGameObject(), Friction, intersection);
            enemyObject->GetGameObject()->move(pushDirection);
            playerController->HealthPoints -= deltaTime;
        }
    }

    shurikenController->_shurikensContainer.erase(std::remove_if(shurikenController->_shurikensContainer.begin(), shurikenController->_shurikensContainer.end(), [](ShurikenObject* shurikenObject) {return shurikenObject->ToRemove; }), shurikenController->_shurikensContainer.end());
    enemyController->SpawnNewEnemies(difficulty, playerController->GetGameObject()->GetCenteredPosition());



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
   

    if (playerController->HealthPoints <= 0)
    {
        isGameRunning = false;
        CurrentGameMode = GameMode::Menu;
        cout << "deaded" << endl;
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
    playerController->GetGameObject()->Update(deltaTime);
    shurikenController->Update(deltaTime);
    enemyController->Update(deltaTime, playerController->GetGameObject()->GetCenteredPosition());
    //cout << "DebugMouse: Moved from " << MouseCursor->getPosition().x << " " << MouseCursor->getPosition().y << endl;
    
}
void LogicLoop(float deltaTime) 
{
    for (int y = 0; y < TileMapHeight; y++)
    {
        for (int x = 0; x < TileMapWidth; x++)
        {
            tileMapArray[x][y]->Update(deltaTime);
        }
    }
}

void UpdateMainView(RenderWindow& mainWindow)
{
    Vector2f newViewCenter = playerController->GetGameObject()->GetCenteredPosition();

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
   for (int y = 0; y < TileMapHeight; y++)
   {
       for (int x = 0; x < TileMapWidth; x++)
       {
         
           tileMapArray[x][y]->SetAnimation(TileMapSet[y][x]);
           tileMapArray[x][y]->setPosition(x * 16, y * 16);
           renderWindow.draw(*tileMapArray[x][y]);
           
       }
   }

   enemyController->DrawEnemy(renderWindow);

    
    renderWindow.draw(*playerController->GetGameObject());

#ifdef DEBUG_DRAW   
    sf::Vertex line[] =
    {
        Vertex(playerController->GetGameObject()->GetCenteredPosition()),
        Vertex(MouseCursor->GetCenteredPosition()),
    };

    renderWindow.draw(line, 2, sf::Lines);
#endif
    shurikenController->DrawShuriken(renderWindow);
    mainText.setPosition(playerController->GetGameObject()->GetCenteredPosition() - Vector2f(mainText.getGlobalBounds().width / 2, (mainText.getGlobalBounds().height / 2) + (24 + 16)));
    renderWindow.draw(mainText);

    scoreText.setFont(algerFont);
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::Black);

    scoreText.setPosition(mainView.getCenter() - Vector2f(160, 105));
    scoreText.setString("Current score: " + std::to_string(score));
    renderWindow.draw(scoreText);



    currentHP.setFont(algerFont);
    currentHP.setCharacterSize(16);
    currentHP.setFillColor(sf::Color::Black);
    currentHP.setPosition(mainView.getCenter() - Vector2f(160, 120));
    stringstream stringStream;
    stringStream << (int)(playerController->HealthPoints * 100);
    currentHP.setString("Current HP: " + stringStream.str());
    renderWindow.draw(currentHP);
    
}


