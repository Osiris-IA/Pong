#include "Game.hpp"
#include <cmath>
#include <string>

Game::Game() : window(sf::VideoMode({WIN_WIDTH, WIN_HEIGHT}), "Pong SFML"),
               font(),
               input(),
               music(),
               txt(font),
               menuText(font),
               currentState(GameState::MENU),
               ballPos(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f),
               ballDir(1.5f, 2.0f),
               ballSpeed(1.0f),
               ball(15.0f),
               posRaquetteLeftY(WIN_HEIGHT / 2),
               posRaquetteRightY(WIN_HEIGHT / 2),
               posRaquetteLeftX(50),
               posRaquetteRightX(WIN_WIDTH - 70),
               raquettesSpeed(5),
               raquettesHeight(150),
               raquettesWidth(20),
               rectangleShapeLeft(sf::Vector2f(raquettesWidth, raquettesHeight)),
               rectangleShapeRight(sf::Vector2f(raquettesWidth, raquettesHeight)),
               scoreJ1(0),
               scoreJ2(0)

{
    window.setFramerateLimit(144);

    if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc"))
        return;

    setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));

    if (!music.openFromFile("assets/Song_Guy.ogg"))
        return;
    music.play();

    ball.setPosition(ballPos);
    rectangleShapeLeft.setPosition(sf::Vector2f(posRaquetteLeftX, posRaquetteLeftY));
    rectangleShapeRight.setPosition(sf::Vector2f(posRaquetteRightX, posRaquetteRightY));

    menuText.setCharacterSize(35);
    menuText.setFillColor(sf::Color::Yellow);
    menuText.setString("      BIENVENUE SUR PONG\n\nAppuyez sur ESPACE pour jouer");
    menuText.setPosition(sf::Vector2f(WIN_WIDTH / 2 - 250, WIN_HEIGHT / 2 - 50));

    mainView.setSize(sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));
    mainView.setCenter(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT / 2));

    obstacle.setSize(sf::Vector2f(40, 100));
    obstacle.setOrigin(sf::Vector2f(20, 50)); // On centre l'origine pour que (400,300) soit le milieu exact
    obstacle.setPosition(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT / 2));
    // ...existing code...
    obstacle.setFillColor(sf::Color::Red);

    if (texFootball.loadFromFile("assets/grass.png"))
    {
        backgroundSprite.emplace(texFootball);
        backgroundSprite->setScale(sf::Vector2f(
            (float)WIN_WIDTH / texFootball.getSize().x,
            (float)WIN_HEIGHT / texFootball.getSize().y));
    }

    // Charge aussi la texture SPACE
    if (!texSpace.loadFromFile("assets/espace.jpg"))
    {
        // Optionnel : gère l'erreur
    }
}

void Game::run()
{
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            input.InputHandler(*event, window);
        }
        update();
        render();
    }
}

void Game::update()
{
    if (currentState == GameState::MENU)
    {
        if (input.GetButton().space == true)
        {
            currentState = GameState::PLAYING;
            ballSpeed = 1.0f;
            ballPos = sf::Vector2f(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
        }
    }
    else if (currentState == GameState::PLAYING)
    {
        if (input.GetButton().p)
            currentState = GameState::PAUSE;
        CheckBtn();
        RaquetteIA();
        UpdateBall();

        if (input.GetButton().escape == true)
        {
            currentState = GameState::MENU;
        }
        rectangleShapeLeft.setPosition(sf::Vector2f(posRaquetteLeftX, posRaquetteLeftY));
        rectangleShapeRight.setPosition(sf::Vector2f(posRaquetteRightX, posRaquetteRightY));
        ball.setPosition(ballPos);
    }
    else if (currentState == GameState::PAUSE)
    {
        if (input.GetButton().enter)
            currentState = GameState::PLAYING;
    }

    // Gestion du changement de thème avec anti-rebond
    static bool tPressedLastFrame = false;
    if (input.GetButton().t && !tPressedLastFrame)
    {
        if (currentTheme == Theme::FOOTBALL)
        {
            currentTheme = Theme::SPACE;
            if (backgroundSprite)
                backgroundSprite->setTexture(texSpace);
        }
        else
        {
            currentTheme = Theme::FOOTBALL;
            if (backgroundSprite)
                backgroundSprite->setTexture(texFootball);
        }
    }
    tPressedLastFrame = input.GetButton().t;

    // Comportements CONTINUS selon le thème
    if (currentTheme == Theme::FOOTBALL)
    {
        // Le "Joueur Adverse" : patrouille verticale au centre
        obstacleTimer += 0.02f;
        float newY = (WIN_HEIGHT / 2) + std::sin(obstacleTimer) * 150.0f;
        obstacle.setPosition(sf::Vector2f(WIN_WIDTH / 2, newY)); // ← Ajoute sf::Vector2f()
    }


    // Shake screen
    if (shakeTimer > 0)
    {
        shakeTimer -= 0.1f;
        float offsetX = (rand() % (int)shakeIntensity) - (shakeIntensity / 2);
        float offsetY = (rand() % (int)shakeIntensity) - (shakeIntensity / 2);
        mainView.setCenter(sf::Vector2f(WIN_WIDTH / 2 + offsetX, WIN_HEIGHT / 2 + offsetY));
    }
    else
    {
        mainView.setCenter(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT / 2));
    }
    window.setView(mainView);
}

void Game::render()
{
    window.clear(sf::Color::Black);

    // Dessine le fond AVANT tout (pour tous les états sauf MENU)
    if (currentState != GameState::MENU && backgroundSprite)
    {
        window.draw(*backgroundSprite);
    }

    if (currentState == GameState::MENU)
    {
        window.draw(menuText);
    }
    else if (currentState == GameState::PLAYING)
    {
        txt.setString(std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
        txt.setPosition(sf::Vector2f((WIN_WIDTH / 2) - 40, 10));

        // Change juste les couleurs selon le thème
        if (currentTheme == Theme::FOOTBALL)
        {
            rectangleShapeLeft.setFillColor(sf::Color::White);
            rectangleShapeRight.setFillColor(sf::Color::White);
            ball.setFillColor(sf::Color::White);
            obstacle.setFillColor(sf::Color::Yellow);
        }
        else if (currentTheme == Theme::SPACE)
        {
            rectangleShapeLeft.setFillColor(sf::Color(0, 255, 255, 200));
            rectangleShapeRight.setFillColor(sf::Color(255, 0, 255, 200));
            ball.setFillColor(sf::Color::Yellow);
            obstacle.setFillColor(sf::Color(100, 100, 255));
        }

        window.draw(txt);
        window.draw(rectangleShapeLeft);
        window.draw(rectangleShapeRight);
        window.draw(ball);
        window.draw(obstacle);
    }
    else if (currentState == GameState::PAUSE)
    {
        window.draw(rectangleShapeLeft);
        window.draw(rectangleShapeRight);
        window.draw(ball);
        txt.setString("PAUSE\nAppuyez sur ENTER pour reprendre");
        txt.setPosition(sf::Vector2f(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2));
        window.draw(txt);
    }

    window.display();
}

// void Game::UpdateBall()
// {
//     ballPos.x += ballDir.x * ballSpeed;
//     ballPos.y += ballDir.y * ballSpeed;

//     if ((ballPos.x < posRaquetteLeftX + raquettesWidth && ballPos.x > posRaquetteLeftX &&
//          ballPos.y < posRaquetteLeftY + raquettesHeight && ballPos.y > posRaquetteLeftY) ||
//         (ballPos.x > posRaquetteRightX - raquettesWidth && ballPos.x < posRaquetteRightX &&
//          (ballPos.y + 7 < posRaquetteRightY + raquettesHeight && ballPos.y + 7 > posRaquetteRightY)))
//     {
//         ballDir.x *= -1;
//         ballSpeed += 0.1f;
//         shakeTimer = 1.0f; // Déclenche le tremblement pendant quelques frames
//     }

//     if (ballPos.x < 0)
//     {
//         scoreJ2++;
//         ballPos = sf::Vector2f(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
//         ballDir.x = std::abs(ballDir.x);
//         ballDir.y *= -1;
//         ballSpeed = 1.5f;
//         shakeTimer = 1.0f; // Déclenche le tremblement pendant quelques frames

//         setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
//     }

//     if (ballPos.x > WIN_WIDTH)
//     {
//         scoreJ1++;
//         ballPos = sf::Vector2f(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
//         ballDir.x = -std::abs(ballDir.x);
//         ballDir.y *= -1;
//         ballSpeed = 1.5f;
//         shakeTimer = 1.0f; // Déclenche le tremblement pendant quelques frames

//         setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
//     }

//     if (ballPos.y > WIN_HEIGHT || ballPos.y < 0)
//         ballDir.y *= -1;
// }

// void Game::RaquetteIA()
// {
//     posRaquetteRightY = static_cast<int>(ballPos.y);
// }

// void Game::CheckBtn()
// {
//     if (input.GetButton().up == true)
//     {
//         posRaquetteLeftY -= raquettesSpeed;
//         if (posRaquetteLeftY < 0)
//             posRaquetteLeftY = 0;
//     }
//     if (input.GetButton().down == true)
//     {
//         posRaquetteLeftY += raquettesSpeed;
//         if (posRaquetteLeftY + raquettesHeight > WIN_HEIGHT)
//             posRaquetteLeftY = WIN_HEIGHT - raquettesHeight;
//     }

//     if (input.GetButton().left == true)
//     {
//         posRaquetteRightY -= raquettesSpeed;
//         if (posRaquetteRightY < 0)
//             posRaquetteRightY = 0;
//     }
//     if (input.GetButton().right == true)
//     {
//         posRaquetteRightY += raquettesSpeed;
//         if (posRaquetteRightY + raquettesHeight > WIN_HEIGHT)
//             posRaquetteRightY = WIN_HEIGHT - raquettesHeight;
//     }
//     if (input.GetButton().escape == true)
//         window.close();
// }

// void Game::setText(sf::Text &txt, sf::String str)
// {
//     txt.setFont(font);
//     txt.setString(str);
//     txt.setCharacterSize(26);
//     txt.setFillColor(sf::Color::White);
//     txt.setPosition(sf::Vector2f((WIN_WIDTH / 2) - 40, 10));
// }