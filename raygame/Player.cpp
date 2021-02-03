#include "Player.h"
#include "Actor.h"
#include "Sprite.h"
#include "raylib.h"

void Player::player(std::string name, std::string area)
{
    setName(name);
    setArea(area);
    setMaxHealth();
    setHealth(playerMaxHealth);
    setDamage();
}

void Player::setName(std::string name)
{
    playerName = name;
}

void Player::setArea(std::string area)
{
    playerArea = area;
}

void Player::setMaxHealth()
{

}
void Player::draw()
{
    
}

void Player::setDamage()
{

}

void Player::setHealth(double health)
{
    playerHealth = health;
}

std::string Player::getName()
{
    return playerName;
}

std::string Player::getArea()
{
    return playerArea;
}

double Player::getHealth()
{
    return playerHealth;
}
double Player::getMaxHealth()
{
    return playerMaxHealth;
}
int Player::getDamage()
{
    return playerDamage;
}
