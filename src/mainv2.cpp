#include "mainV2.hpp"

RenderWindow window;

Input input;

Font font;

int posRaquetteLeftX = 50;
int posRaquetteRightX = WIN_WIDTH - 70;
int posRaquetteLeftY = WIN_HEIGHT / 2;
int posRaquetteRightY = posRaquetteLeftY;
int raquettesSpeed = 5;
int raquettesHeight = 150;
int raquettesWidth = 20;

float ballSpeed = 1;

Vector2f ballDir = Vector2f(1.5f, 2);
float ballPosX = WIN_WIDTH / 2;
float ballPosY = WIN_HEIGHT / 2;

int scoreJ1 = 0;
int scoreJ2 = 0;

void RaquetteIA()
{
    posRaquetteRightY = ballPosY;
}

void UpdateBall(sf::Text &txt)
{
    ballPosX += ballDir.x * ballSpeed;
    ballPosY += ballDir.y * ballSpeed;

    // collision
    // raquette gauche ou droite touchée ?
    if ((ballPosX < posRaquetteLeftX + raquettesWidth && ballPosX > posRaquetteLeftX &&
         ballPosY < posRaquetteLeftY + raquettesHeight && ballPosY > posRaquetteLeftY) ||
        (ballPosX > posRaquetteRightX - raquettesWidth && ballPosX < posRaquetteRightX &&
         (ballPosY + 7 < posRaquetteRightY + raquettesHeight && ballPosY + 7 > posRaquetteRightY)))
    {
        ballDir.x *= -1;
    }

    // mur de gauche

    if (ballPosX < 0)
    {
        scoreJ2++;
        ballPosX = WIN_WIDTH / 2;
        ballPosY = WIN_HEIGHT / 2;
        ballDir.x = fabs(ballDir.x); // valeur absolut pour avoir une valeur positif donc forcément à droite
        ballDir.y *= -1;
        setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
    }

    // mur droite
    if (ballPosX > WIN_WIDTH)
    {
        scoreJ1++;
        ballPosX = WIN_WIDTH / 2;
        ballPosY = WIN_HEIGHT / 2;
        ballDir.x = -fabs(ballDir.x); // valeur absolut pour avoir une valeur négatif donc en direction de la gauche
        ballDir.y *= -1;
        setText(txt, std::to_string(scoreJ1) + " | " + std::to_string(scoreJ2));
    }

    // mur haut ou bas

    if (ballPosY > WIN_HEIGHT || ballPosY < 0)
    {
        ballDir.y *= -1;
    }
}

int main()
{
    RenderWindow window = RenderWindow(VideoMode({WIN_WIDTH, WIN_HEIGHT}), "Pong SFML");
    window.setFramerateLimit(144);

    if (!font.openFromFile("/System/Library/Fonts/Helvetica.ttc"))
        return 1;

    Text txt(font);
    setText(txt, to_string(scoreJ1) + " | " + to_string(scoreJ2));

    Music music;
    if (!music.openFromFile("assets/Song_Guy.ogg"))
        return 1;
    music.play();

    // balle
    CircleShape circleShape(15);
    circleShape.setPosition(Vector2f(ballPosX, ballPosY));

    // raquette gauche
    RectangleShape rectangleShapeLeft(Vector2f(raquettesWidth, raquettesHeight));
    rectangleShapeLeft.setPosition(Vector2f(posRaquetteLeftX, posRaquetteLeftY));

    // raquette droite
    RectangleShape rectangleShapeRight(Vector2f(raquettesWidth, raquettesHeight));
    rectangleShapeRight.setPosition(Vector2f(posRaquetteRightX, posRaquetteRightY));

    // Boucle
    while (window.isOpen())
    {
        while (const std::optional<Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close(); // Ferme si on clique sur X
            input.InputHandler(*event, window);
        }
        CheckBtn();
        RaquetteIA();

        // position raquette, balle

        rectangleShapeLeft.setPosition(Vector2f(posRaquetteLeftX, posRaquetteLeftY));
        rectangleShapeRight.setPosition(Vector2f(posRaquetteRightX, posRaquetteRightY));

        // update balle

        UpdateBall(txt);
        circleShape.setPosition(Vector2f(ballPosX, ballPosY));

        window.clear(Color::Black);

        window.draw(txt);
        window.draw(rectangleShapeLeft);
        window.draw(rectangleShapeRight);
        window.draw(circleShape);

        window.display();
    }
    return 0;
}

void setText(sf::Text &txt, sf::String str)
{
    txt.setFont(font);
    txt.setString(str);
    txt.setCharacterSize(26);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(sf::Vector2f((WIN_WIDTH / 2) - 40, 10));
}

void CheckBtn()
{
    // raquette gauche
    if (input.GetButton().up == true)
    {
        posRaquetteLeftY -= raquettesSpeed;
        if (posRaquetteLeftY < 0)
        {
            posRaquetteLeftY = 0;
        }
    }
    if (input.GetButton().down == true)
    {
        posRaquetteLeftY += raquettesSpeed;
        if (posRaquetteLeftY + raquettesHeight > WIN_HEIGHT)
        {
            posRaquetteLeftY = WIN_HEIGHT - raquettesHeight;
        }
    }
    // raquette droite

    if (input.GetButton().left == true)
    {
        posRaquetteRightY -= raquettesSpeed;
        if (posRaquetteRightY < 0)
        {
            posRaquetteRightY = 0;
        }
    }
    if (input.GetButton().right == true)
    {
        posRaquetteRightY += raquettesSpeed;
        if (posRaquetteRightY + raquettesHeight > WIN_HEIGHT)
        {
            posRaquetteRightY = WIN_HEIGHT - raquettesHeight;
        }
    }
    if (input.GetButton().escape == true)
    {
        window.close();
    }
}