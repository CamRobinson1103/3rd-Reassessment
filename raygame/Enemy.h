#include "Actor.h"
#include<string>
class Enemy
{
public:
    void enemy(std::string);
    void setArea(std::string);
    void setMaxHealth();
    void setDamage();
    std::string getArea();
    void setHealth(double);
    double getHealth();
    double getMaxHealth();
    int getDamage();

private:
    std::string enemyArea;
    int enemyLevel;
    double enemyHealth;
    double enemyMaxHealth;
    int enemyDamage;
    Sprite* m_sprite;
};

