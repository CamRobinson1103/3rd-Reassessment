#include "Actor.h"
#include "raylib.h"
#include<string>
class Player
{
public:
    Player(float x, float y, Color rayColor, char icon = ' ' );
    void player(std::string, std::string);
    void setName(std::string);
    void setArea(std::string);
    void setMaxHealth();
    void setDamage();
    void draw();
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
};

