#include "Game.h"
#include "raylib.h"
#include "Vector2.h"
#include "Actor.h"
#include <math.h>
#include <stdlib.h>
#include <ctime>
#define NUM_SHOOTS 50


bool Game::m_gameOver = false;
bool collision = false;
bool pause = false;
bool _canMove = true;
Scene** Game::m_scenes = new Scene*;
int Game::m_sceneCount = 0;
int Game::m_currentSceneIndex = 0;

void DisableControls()
{
	_canMove = false;
}

typedef struct Player {
	Rectangle rec;
	Vector2 speed;
	bool active;
	Color color;
} Player;

typedef struct Shoot {
	Rectangle rec;
	Vector2 speed;
	bool active;
	Color color;
} Shoot;


static Player player1 = { 0 };
static Player player2 = { 0 };
static Shoot shoot = { 0 };
static int shootRate = 0;
static int activePlayers = 2;
static int playerKill = 0;
static float alpha = 0.0f;
static int activeEnemies = 0;
static int enemiesKill = 0;
static bool smooth = false;

Game::Game()
{
	m_gameOver = false;
	shootRate = 0;
	m_scenes = new Scene*;
	m_camera = new Camera2D();
	m_currentSceneIndex = 0;
	m_sceneCount = 0;
}

void Game::start()
{
	const int screenWidth = 1024;
	const int screenHeight = 760;

	player1.rec.x = 1000;
	player1.rec.y = 50;
	player1.rec.width = 100;
	player1.rec.height = 100;
	player1.speed.x = 1;
	player1.speed.y = 1;
	player1.color = PURPLE;

	player2.rec.x = 100;
	player2.rec.y = 50;
	player2.rec.width = 100;
	player2.rec.height = 100;
	player2.speed.x = 1;
	player2.speed.y = 1;
	player2.color = YELLOW;

	shoot.rec.x = 100;
	shoot.rec.y = 50;
	shoot.rec.width = 100;
	shoot.rec.height = 100;
	shoot.speed.x = 1;
	shoot.speed.y = 1;
	shoot.color = GREEN;

	static Shoot shoot[NUM_SHOOTS] = { 0 };

	for (int i = 0; i < NUM_SHOOTS; i++)
	{
		shoot[i].rec.x = player1.rec.x;
		shoot[i].rec.y = player1.rec.y + player1.rec.height / 4;
		shoot[i].rec.width = 10;
		shoot[i].rec.height = 5;
		shoot[i].speed.x = 7;
		shoot[i].speed.y = 3;
		shoot[i].active = false;
		shoot[i].color = MAROON;
	}

	

	InitWindow(screenWidth, screenHeight, "Yellow vs. Purple");
	m_camera->offset = { (float)screenWidth / 2, (float)screenHeight / 2 };
	m_camera->target = { (float)screenWidth / 2, (float)screenHeight / 2 };
	m_camera->zoom = 1;

	Vector2 purpleBoxPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
	Vector2 yellowBoxPosition = { (float)screenWidth / 1, (float)screenHeight / 2 };
	Vector2 purpleBallSpeed{};
	Vector2 yellowBallSpeed{};
	



	while (!WindowShouldClose())    
	{
		if (m_gameOver == false)
		{
			Actor* purpleBox = new Actor(5, 5, 50, '■', 5);
			Actor* yellowBox = new Actor(5, 5, 50, '■', 1);

			Rectangle boxA = { GetScreenWidth() / 2 , GetScreenHeight() / 2 - 30, 60,60 };
			Vector2 purpBox = { (float)screenWidth / 1, (float)screenHeight / 2 };
			Rectangle boxB = { GetScreenWidth() / 1 , GetScreenHeight() / 2 - 30, 60, 60 };
			Vector2 yellBox = { (float)screenWidth / 1, (float)screenHeight / 2 };


			Rectangle boxCollision = { 5 };

			if (IsKeyDown(KEY_D)) player2.rec.x += player2.speed.x;
			if (IsKeyDown(KEY_A)) player2.rec.x -= player2.speed.x;
			if (IsKeyDown(KEY_W)) player2.rec.y -= player2.speed.y;
			if (IsKeyDown(KEY_S)) player2.rec.y += player2.speed.y;



			if (IsKeyDown(KEY_RIGHT)) player1.rec.x += player1.speed.x;
			if (IsKeyDown(KEY_LEFT)) player1.rec.x -= player1.speed.x;
			if (IsKeyDown(KEY_UP)) player1.rec.y -= player1.speed.y;
			if (IsKeyDown(KEY_DOWN)) player1.rec.y += player1.speed.y;

			if (IsKeyDown(KEY_SPACE))
			{
				DrawRectangleRec(shoot->rec, shoot->color);
				shootRate += 5;

				for (int i = 0; i < NUM_SHOOTS; i++)
				{
					if (!shoot[i].active && shootRate % 20 == 0)
					{
						shoot[i].rec.x = player1.rec.x;
						shoot[i].rec.y = player1.rec.y + player1.rec.height / 4;
						shoot[i].active = true;
						break;
					}
				}
			}

			
		



			if ((boxB.x + boxB.width) >= GetScreenWidth()) boxB.x = GetScreenWidth() - boxB.width;
			else if (boxB.x <= 0) boxB.x = 0;

			if ((boxB.y + boxB.height) >= GetScreenHeight()) boxB.y = GetScreenHeight() - boxB.height;


			if ((purpleBoxPosition.x >= (GetScreenWidth() - purpleBox->getCollisionRadius())) || (purpleBoxPosition.x <= purpleBox->getCollisionRadius())) purpleBallSpeed.x *= -1.0f;
			if ((purpleBoxPosition.y >= (GetScreenHeight() - purpleBox->getCollisionRadius())) || (purpleBoxPosition.y <= purpleBox->getCollisionRadius())) purpleBallSpeed.y *= -1.0f;

			if ((yellowBoxPosition.x >= (GetScreenWidth() - yellowBox->getCollisionRadius())) || (yellowBoxPosition.x <= yellowBox->getCollisionRadius())) yellowBallSpeed.x *= -1.0f;
			if ((yellowBoxPosition.y >= (GetScreenHeight() - yellowBox->getCollisionRadius())) || (yellowBoxPosition.y <= yellowBox->getCollisionRadius())) yellowBallSpeed.y *= -1.0f;

			collision = CheckCollisionRecs(player1.rec, player2.rec);

			// Get collision rectangle (only on collision)
			if (collision) boxCollision = GetCollisionRec(boxA, boxB);


			purpleBox->onCollision(yellowBox);

			if (collision)
			{
				m_gameOver == true;
				pause = true;
				ClearBackground(RAYWHITE);
				DrawText("You Win!", screenWidth / 2 - MeasureText("You Win!", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
				player2.color = RAYWHITE;
				player1.color = BLACK;
				
				_canMove = false;

			}
			if (collision)
			{

			}
		}
		
		
		BeginDrawing();
		DrawRectangleRec(player1.rec, player1.color);
		DrawRectangleRec(player2.rec, player2.color);
		

		ClearBackground(BLACK);

		EndDrawing();
	}

	SetTargetFPS(60);
}



void Game::update(float deltaTime)
{
	for (int i = 0; i < m_sceneCount; i++)
	{
		m_scenes[i]->update(deltaTime);
	}
}

void Game::draw()
{
	BeginDrawing();

	BeginMode2D(*m_camera);
	ClearBackground(BLACK);

	

	for (int i = 0; i < m_sceneCount; i++)
	{
		m_scenes[i]->draw();
	}

	EndMode2D();
	EndDrawing();
}

void Game::end()
{
	CloseWindow();
}

MathLibrary::Matrix3* Game::getWorld()
{
	return getCurrentScene()->getWorld();
}

void Game::run()
{
	start();

	while (!m_gameOver && !RAYLIB_H::WindowShouldClose())
	{

		float deltaTime = RAYLIB_H::GetFrameTime();
		update(deltaTime);
		draw();
	}

	end();
}

Scene* Game::getScene(int index)
{
	if (index < 0 || index >= m_sceneCount)
		return nullptr;

	return m_scenes[index];
}

Scene* Game::getCurrentScene()
{
	return m_scenes[m_currentSceneIndex];
}

int Game::getCurrentSceneIndex()
{
	return m_currentSceneIndex;
}

int Game::addScene(Scene* scene)
{
	//If the scene is null then return before running any other logic
	if (!scene)
		return -1;

	//Create a new temporary array that one size larger than the original
	Scene** tempArray = new Scene*[m_sceneCount + 1];

	//Copy values from old array into new array
	for (int i = 0; i < m_sceneCount; i++)
	{
		tempArray[i] = m_scenes[i];
	}

	//Store the current index
	int index = m_sceneCount;

	//Sets the scene at the new index to be the scene passed in
	tempArray[index] = scene;

	//Set the old array to the tmeporary array
	m_scenes = tempArray;
	m_sceneCount++;

	return index;
}

bool Game::removeScene(Scene* scene)
{
	//If the scene is null then return before running any other logic
	if (!scene)
		return false;

	bool sceneRemoved = false;

	//Create a new temporary array that is one less than our original array
	Scene** tempArray = new Scene*[m_sceneCount];

	//Copy all scenes except the scene we don't want into the new array
	int j = 0;
	for (int i = 0; i < m_sceneCount; i++)
	{
		if (tempArray[i] != scene)
		{
			tempArray[j] = m_scenes[i];
			j++;
		}
		else
		{
			sceneRemoved = true;
		}
	}

	//If the scene was successfully removed set the old array to be the new array
	if (sceneRemoved)
	{
		m_scenes = tempArray;
		m_sceneCount--;
	}
		

	return sceneRemoved;
}

void Game::setCurrentScene(int index)
{
	//If the index is not within the range of the the array return
	if (index < 0 || index >= m_sceneCount)
		return;

	//Call end for the previous scene before changing to the new one
	if (m_scenes[m_currentSceneIndex]->getStarted())
		m_scenes[m_currentSceneIndex]->end();

	//Update the current scene index
	m_currentSceneIndex = index;
}

bool Game::getKeyDown(int key)
{
	return RAYLIB_H::IsKeyDown((KeyboardKey)key);
}

bool Game::getKeyPressed(int key)
{
	return RAYLIB_H::IsKeyPressed((KeyboardKey)key);
}

void Game::destroy(Actor* actor)
{
	getCurrentScene()->removeActor(actor);
	if (actor->getParent())
		actor->getParent()->removeChild(actor);
	actor->end();
	delete actor;
}

void Game::setGameOver(bool value)
{
	Game::m_gameOver = value;
}
