#ifndef GAME_HPP
#define GAME_HPP

#include "Constants.hpp"
#include "input.hpp"
#include <SFML/Audio.hpp>
#include <optional>

enum class GameState
{
    MENU,
    PLAYING,
    GAME_OVER,
    PAUSE
};

enum class Theme
{
    FOOTBALL,
    SPACE
};

class Game
{
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    sf::Font font;
    Input input;
    sf::Music music;
    sf::Text txt;
    sf::Text menuText;

    GameState currentState;

    sf::View mainView;
    float shakeTimer = 0.0f;
    float shakeIntensity = 5.0f;
    float obstacleTimer = 0.0f;

    Theme currentTheme = Theme::FOOTBALL; 

    sf::Texture texFootball;
    std::optional<sf::Sprite> backgroundSprite;
    sf::Texture texSpace;

    sf::Texture texBallSpace;

    // ball
    sf::Vector2f ballPos;

    sf::Vector2f ballDir;
    float ballSpeed;
    sf::CircleShape ball;

    // Paddles
    int posRaquetteLeftY;
    int posRaquetteRightY;
    int posRaquetteLeftX;
    int posRaquetteRightX;
    int raquettesSpeed;
    int raquettesHeight;
    int raquettesWidth;
    sf::RectangleShape rectangleShapeLeft;
    sf::RectangleShape rectangleShapeRight;

    sf::RectangleShape obstacle;

    // Score
    int scoreJ1;
    int scoreJ2;

    void UpdateBall();
    void RaquetteIA();
    void CheckBtn();
    void setText(sf::Text &txt, sf::String str);
    void update();
    void render();
};

#endif