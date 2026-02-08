#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Graphics.hpp>

class Input
{
public:
    struct Button
    {
        bool left, right, up, down, escape, space, p, enter, t;
    };

    // Proto du constructeur
    Input();

    const Button &GetButton(void) const; // return l'état d'un boutton
    void InputHandler(const sf::Event &event, sf::RenderWindow &window);

private:
    Button button;

};

#endif