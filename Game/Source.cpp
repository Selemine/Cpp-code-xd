#include <SFML/Graphics.hpp>
#include <vector>
#include <windows.h>
#include <iostream>
#include <thread>
using namespace std;
using namespace sf;
class Stars {
private:
    vector<CircleShape> stars;
public:
    Stars(int nStars, int wWidht, int wHeight) {
        for (int i = 0; i < nStars; i++) {
            float x = rand() % wWidht + 1;
            float y = rand() % wHeight + 1;
            bool isWhite = (i % 2 == 0);
            CircleShape shape(1, 5);
            shape.setFillColor(isWhite ? Color::White : Color::Black);
            shape.setPosition(x, y);
            stars.push_back(shape);
        }
    }
    void drawStars(RenderWindow& window) {
        int pos[400];
        for (int i = 0; i < 400; i++) {
            pos[i] = rand() % 400;
            if (i % 2 == 0) {
                stars[pos[i]].setFillColor(Color::Black);
            }
            else {
                stars[pos[i]].setFillColor(Color::White);
            }
            window.draw(stars[i]);
        }
    }

};





class Hero {
private:
    Texture herotexture;
    Sprite herosprite;
    std::vector<IntRect> textureCoords;
public:
    Hero() {
        herotexture.loadFromFile("images/first.png");
        herosprite.setTexture(herotexture);
        textureCoords.push_back(IntRect(75, 2358, 105, 105));   //0
        textureCoords.push_back(IntRect(60, 66, 135, 112));     //1
        textureCoords.push_back(IntRect(25, 1046, 205, 180));     //2

        setTexture(2);

        sf::FloatRect bounds = herosprite.getLocalBounds();
        herosprite.setOrigin(bounds.width / 2, bounds.height / 2);
        herosprite.setPosition(600, 800);
        herosprite.setScale(0.8f, 0.8f);

    }

    void drawHero(RenderWindow& window) {
        float heroSpeed = 0.2;
        sf::Vector2u screenSize = window.getSize();
        sf::Vector2f heroPosition = herosprite.getPosition();



        if (Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A))) {
            if (heroPosition.x > 0) {
                herosprite.move(-heroSpeed, 0);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))) {
            if (heroPosition.x < screenSize.x - 1) {
                herosprite.move(heroSpeed, 0);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W))) {
            if (heroPosition.y > 0) {
                herosprite.move(0, -heroSpeed);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S))) {
            if (heroPosition.y < screenSize.y - 25) {
                herosprite.move(0, heroSpeed);
            }
        }
        window.draw(herosprite);
    }

    void setTexture(int textureIndex) {
        if (textureIndex >= 0 && textureIndex < textureCoords.size()) {
            herosprite.setTextureRect(textureCoords[textureIndex]);
        }
    }

    Sprite& getHeroSprite() {
        return herosprite;
    }
};


class Ammo {
private:
    Texture ammotexture;
    Sprite ammosprite;
    Hero* hero;
    
    std::vector<IntRect> textureCoords;
    vector<Sprite> bullets;
    bool canShot = true;
    

    float AmmoDMG;

public:
    Ammo(Hero* heroPtr) : hero(heroPtr) {
        ammotexture.loadFromFile("images/first.png");
        ammosprite.setTexture(ammotexture);
        textureCoords.push_back(IntRect(357, 2467, 55, 55));
        textureCoords.push_back(IntRect(316, 413, 136, 72));
        textureCoords.push_back(IntRect(315, 693, 135, 119));

        AmmoDMG = 34;
        setTexture(2);

        sf::FloatRect bounds = ammosprite.getLocalBounds();
        ammosprite.setOrigin(bounds.width / 2, bounds.height / 2);
        ammosprite.setScale(0.75f, 0.75f);
    }

    void drawAmmo(RenderWindow& window) {

        if (Mouse::isButtonPressed(Mouse::Left) && canShot) {
            Vector2f herospriteSize = hero->getHeroSprite().getPosition();
            ammosprite.setPosition((herospriteSize.x), (herospriteSize.y) - 80);
            bullets.push_back(ammosprite);
            canShot = false;
        }
        if (!Mouse::isButtonPressed(Mouse::Left)) {
            canShot = true;
        }
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].move(0, -1);
            window.draw(bullets[i]);

            if (bullets[i].getPosition().y <= 0) {
                bullets.erase(bullets.begin() + i);
            }
           
        }
    }
    
    void setTexture(int textureIndex) {
        if (textureIndex >= 0 && textureIndex < textureCoords.size()) {
            ammosprite.setTextureRect(textureCoords[textureIndex]);
        }
    }

};




class Enemy {
private:
    Sprite enemysprite;
    Texture enemytexture;
    vector<IntRect> enemytextureCoords;
    RectangleShape healthBar;
    float HealthEnemyOne;
    float MaxHealthEnemyOne;

    bool drawSprite = true;

public:
    Enemy() {
        if (drawSprite) {
            enemytexture.loadFromFile("images/first.png");
            enemysprite.setTexture(enemytexture);
            enemytextureCoords.push_back(IntRect(59, 317, 143, 140));   //0
            setTexture(0);

            HealthEnemyOne = 100;
            MaxHealthEnemyOne = 100;

            sf::FloatRect bounds = enemysprite.getLocalBounds();
            enemysprite.setOrigin(bounds.width / 2, bounds.height / 2);
            enemysprite.setScale(0.8f, -0.8f);

            healthBar.setSize(sf::Vector2f(100.0f, 10.0f));
            healthBar.setFillColor(sf::Color::Green);
        }
    }

   

    void setTexture(int textureIndex) {
        if (textureIndex >= 0 && textureIndex < enemytextureCoords.size()) {
            enemysprite.setTextureRect(enemytextureCoords[textureIndex]);
        }
    }

    

    void drawHealthBar(RenderWindow& window) {
        float healthPercentage = HealthEnemyOne / MaxHealthEnemyOne;
        healthBar.setSize(sf::Vector2f(100.0f * healthPercentage, 10.0f));

        sf::Vector2f position(enemysprite.getPosition().x - healthBar.getSize().x / 2, enemysprite.getPosition().y + enemysprite.getGlobalBounds().height / 2 + 10.0f);
        healthBar.setPosition(position);
        window.draw(healthBar);
    }


    void drawEnemy(RenderWindow& window) {
        enemysprite.setPosition(600, 100);
        window.draw(enemysprite);
    }

    Sprite& getEnemySprite() {
        return enemysprite;
    }
};

int main() {
    RenderWindow window(VideoMode(1200, 900), "fxck space");
    sf::Vector2u screenSize = window.getSize();
    Stars stars(400, screenSize.x, screenSize.y);

    Hero hero;
    Enemy enemy;
    Ammo ammo(&hero);

    



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        stars.drawStars(window);
        hero.drawHero(window);
        ammo.drawAmmo(window);
       

        enemy.drawEnemy(window);
        enemy.drawHealthBar(window);




        window.display();
    }
}







