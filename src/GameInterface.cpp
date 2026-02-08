#include "Game.hpp"

void Game::setText(sf::Text &txt, sf::String str)
{
    txt.setFont(font);
    txt.setString(str);
    txt.setCharacterSize(26);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(sf::Vector2f((WIN_WIDTH / 2) - 40, 10));
}