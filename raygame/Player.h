#include "Actor.h"
#include<string>
class Player
{
public:
    void player(std::string, std::string);
    void setName(std::string);
    void setArea(std::string);
    void setMaxHealth();
    void setDamage();
    std::string getName();
    std::string getArea();
    void setHealth(double);
    double getHealth();
    double getMaxHealth();
    int getDamage();

private:
    std::string playerName;
    std::string playerArea;
    int playerLevel;
    double playerHealth;
    double playerMaxHealth;
    int playerDamage;
    Sprite* m_sprite;
};

