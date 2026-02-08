#include "input.hpp"

// constructeur
Input::Input()
{
    button.left = button.right = button.up = button.down = button.escape = button.space = button.p = button.enter = button.t = false;
}

const Input::Button &Input::GetButton(void) const
{
    return button;
}

void Input::InputHandler(const sf::Event &event, sf::RenderWindow &window)
{
    if (event.is<sf::Event::Closed>())
    {
        window.close();
    }

    if (const auto *key = event.getIf<sf::Event::KeyPressed>())
    {
        switch (key->code)
        {
        case sf::Keyboard::Key::Escape:
            button.escape = true;
            break;
        case sf::Keyboard::Key::Space:
            button.space = true;
            break;
        case sf::Keyboard::Key::Left:
            button.left = true;
            break;
        case sf::Keyboard::Key::Right:
            button.right = true;
            break;
        case sf::Keyboard::Key::Down:
            button.down = true;
            break;
        case sf::Keyboard::Key::Up:
            button.up = true;
            break;
        case sf::Keyboard::Key::P:
            button.p = true;
            break;
        case sf::Keyboard::Key::Enter:
            button.enter = true;
            break;
        case sf::Keyboard::Key::T:
            button.t = true;
            break;
        default:
            break;
        }
    }

    if (const auto *key = event.getIf<sf::Event::KeyReleased>())
    {
        switch (key->code)
        {

        case sf::Keyboard::Key::Space:
            button.space = false;
            break;
        case sf::Keyboard::Key::Left:
            button.left = false;
            break;
        case sf::Keyboard::Key::Right:
            button.right = false;
            break;
        case sf::Keyboard::Key::Down:
            button.down = false;
            break;
        case sf::Keyboard::Key::Up:
            button.up = false;
            break;
        case sf::Keyboard::Key::P:
            button.p = false;
            break;
        case sf::Keyboard::Key::Enter:
            button.enter = false;
            break;
        case sf::Keyboard::Key::T:
            button.t = false;
            break;
        case sf::Keyboard::Key::Escape:
            button.escape = false;
            break;

        default:
            break;
        }
    }
}
