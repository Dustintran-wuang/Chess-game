#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Project");
    window.setFramerateLimit(60); 


    sf::CircleShape shape(100.f); 
    shape.setFillColor(sf::Color::Green); 
    
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(shape);

        window.display();
    }

    return 0;
}