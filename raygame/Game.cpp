#include "Game.h"
#include "raylib.h"
#include "Vector2.h"
#include "Actor.h"
#include <math.h>


bool Game::m_gameOver = false;
bool collision = false;
bool pause = false;
Scene** Game::m_scenes = new Scene*;
int Game::m_sceneCount = 0;
int Game::m_currentSceneIndex = 0;




Game::Game()
{
	m_gameOver = false;
	m_scenes = new Scene*;
	m_camera = new Camera2D();
	m_currentSceneIndex = 0;
	m_sceneCount = 0;
}

void Game::start()
{
	const int screenWidth = 1024;
	const int screenHeight = 760;

	

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

		Actor* purpleBox = new Actor(5, 5, 50, '■', 5);
		Actor* yellowBox = new Actor(5, 5, 50, '■', 1);

		Rectangle boxA = { GetScreenWidth() / 2 , GetScreenHeight() / 2 -30, 60,60  };
		Vector2 purpBox = { (float)screenWidth / 1, (float)screenHeight / 2 };
		Rectangle boxB = { GetScreenWidth() / 1 , GetScreenHeight() / 2 - 30, 60, 60 };
		Vector2 yellBox = { (float)screenWidth / 1, (float)screenHeight / 2 };


		Rectangle boxCollision = { 0 }; 

		if (IsKeyDown(KEY_D)) boxA.x += 450.0f;
		if (IsKeyDown(KEY_A)) boxA.x -= 2.0f;
		if (IsKeyDown(KEY_W)) boxA.y -= 2.0f;
		if (IsKeyDown(KEY_S)) boxA.y += 2.0f;
		
		if (IsKeyDown(KEY_RIGHT)) boxB.x += 2.0f;
		if (IsKeyDown(KEY_LEFT)) boxB.x -= 450.0f;
		if (IsKeyDown(KEY_UP)) boxB.y -= 2.0f;
		if (IsKeyDown(KEY_DOWN)) boxB.y += 2.0f;

	


		if ((boxB.x + boxB.width) >= GetScreenWidth()) boxB.x = GetScreenWidth() - boxB.width;
		else if (boxB.x <= 0) boxB.x = 0;

		if ((boxB.y + boxB.height) >= GetScreenHeight()) boxB.y = GetScreenHeight() - boxB.height;


		if ((purpleBoxPosition.x >= (GetScreenWidth() - purpleBox->getCollisionRadius())) || (purpleBoxPosition.x <= purpleBox->getCollisionRadius())) purpleBallSpeed.x *= -1.0f;
		if ((purpleBoxPosition.y >= (GetScreenHeight() - purpleBox->getCollisionRadius())) || (purpleBoxPosition.y <= purpleBox->getCollisionRadius())) purpleBallSpeed.y *= -1.0f;

		if ((yellowBoxPosition.x >= (GetScreenWidth() - yellowBox->getCollisionRadius())) || (yellowBoxPosition.x <= yellowBox->getCollisionRadius())) yellowBallSpeed.x *= -1.0f;
		if ((yellowBoxPosition.y >= (GetScreenHeight() - yellowBox->getCollisionRadius())) || (yellowBoxPosition.y <= yellowBox->getCollisionRadius())) yellowBallSpeed.y *= -1.0f;
		
		collision = CheckCollisionRecs(boxA, boxB);

		// Get collision rectangle (only on collision)
		if (collision) boxCollision = GetCollisionRec(boxA, boxB);


		purpleBox->onCollision(yellowBox);
		
		if (collision)
		{
			DrawText("You will! Press esc to close window", 2, 2, 35, RED);

			m_gameOver = true;
			
		}
		
		
		
		BeginDrawing();
		DrawRectanglePro(boxA, purpBox, 20, PURPLE);
		DrawRectanglePro(boxB, yellBox, 20 ,YELLOW);
		DrawCircleV(purpleBoxPosition, 25, MAROON);
		
	
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
