#include "Game.hpp"
#include <cmath>

void Game::UpdateBall()
{
    ballPos.x += ballDir.x * ballSpeed;
    ballPos.y += ballDir.y * ballSpeed;

    if ((ballPos.x < posRaquetteLeftX + raquettesWidth && ballPos.x > posRaquetteLeftX &&
         ballPos.y < posRaquetteLeftY + raquettesHeight && ballPos.y > posRaquetteLeftY) ||
        (ballPos.x > posRaquetteRightX - raquettesWidth && ballPos.x < posRaquetteRightX &&
         (ballPos.y + 7 < posRaquetteRightY + raquettesHeight && ballPos.y + 7 > posRaquetteRightY)))
    {
        ballDir.x *= -1;
        ballSpeed += 0.1f;
        shakeTimer = 1.0f; // Déclenche le tremblement pendant quelques frames
    }

    if (ballPos.x < 0)
    {
        scoreJ2++;
        ballPos = sf::Vector2f(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
        ballDir.x = std::abs(ballDir.x);
        ballDir.y *= -1;
        ballSpeed = 1.5f;
        shakeTimer = 1.0f; // Déclenche le tremblement pendant quelques frames

        setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
    }

    if (ballPos.x > WIN_WIDTH)
    {
        scoreJ1++;
        ballPos = sf::Vector2f(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
        ballDir.x = -std::abs(ballDir.x);
        ballDir.y *= -1;
        ballSpeed = 1.5f;
        shakeTimer = 1.0f; // Déclenche le tremblement pendant quelques frames

        setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
    }

    if (ballPos.y > WIN_HEIGHT || ballPos.y < 0)
        ballDir.y *= -1;

    if (ball.getGlobalBounds().findIntersection(obstacle.getGlobalBounds()))
    {
        ballDir.x *= -1; // Rebond horizontal
        // On peut aussi secouer l'écran ici !
        shakeTimer = 1.0f; //
    }
}

void Game::RaquetteIA()
{
    // posRaquetteRightY = static_cast<int>(ballPos.y);
    float vitesseIA = 2.5f;
    if (posRaquetteRightY + (raquettesHeight / 2) < ballPos.y)
    {
        posRaquetteRightY += vitesseIA;
    }
    else if (posRaquetteRightY + (raquettesHeight / 2) > ballPos.y)
    {
        posRaquetteRightY -= vitesseIA;
    }
}

void Game::CheckBtn()
{
    if (input.GetButton().up == true)
    {
        posRaquetteLeftY -= raquettesSpeed;
        if (posRaquetteLeftY < 0)
            posRaquetteLeftY = 0;
    }
    if (input.GetButton().down == true)
    {
        posRaquetteLeftY += raquettesSpeed;
        if (posRaquetteLeftY + raquettesHeight > WIN_HEIGHT)
            posRaquetteLeftY = WIN_HEIGHT - raquettesHeight;
    }

    if (input.GetButton().left == true)
    {
        posRaquetteRightY -= raquettesSpeed;
        if (posRaquetteRightY < 0)
            posRaquetteRightY = 0;
    }
    if (input.GetButton().right == true)
    {
        posRaquetteRightY += raquettesSpeed;
        if (posRaquetteRightY + raquettesHeight > WIN_HEIGHT)
            posRaquetteRightY = WIN_HEIGHT - raquettesHeight;
    }
    if (input.GetButton().escape == true)
        window.close();
}