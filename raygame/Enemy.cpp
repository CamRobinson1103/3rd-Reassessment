#include "Enemy.h"
#include "Actor.h"
#include "Sprite.h"

void Enemy::enemy(std::string area)
{
    setArea(area);
    setMaxHealth();
    setHealth(enemyMaxHealth);
    setDamage();
}


void Enemy::setArea(std::string area)
{
    enemyArea = area;
}

void Enemy::setMaxHealth()
{
}


void Enemy::setDamage()
{
}

void Enemy::setHealth(double health)
{
    enemyHealth = health;
}


std::string Enemy::getArea()
{
    return enemyArea;
}

double Enemy::getHealth()
{
    return enemyHealth;
}
double Enemy::getMaxHealth()
{
    return enemyMaxHealth;
}
int Enemy::getDamage()
{
    return enemyDamage;
}

