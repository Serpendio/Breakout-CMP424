#pragma once
#include <SFML/Graphics.hpp>
#include "json.hpp"

class Brick {
public:
    Brick(float x, float y, float width, float height);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    nlohmann::json getData();

private:
    sf::RectangleShape _shape;
    bool _isDestroyed;
};