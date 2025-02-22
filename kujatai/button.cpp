#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& text, sf::Font& font) {
    buttonShape.setSize(sf::Vector2f(width, height));
    buttonShape.setPosition(x, y);
    buttonShape.setFillColor(sf::Color::White);

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(30); // ???????????????????????????????
    buttonText.setFillColor(sf::Color::Black);

    // ?????????????????????
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    float textX = x + (width - textBounds.width) / 2 - textBounds.left;
    float textY = y + (height - textBounds.height) / 2 - textBounds.top;
    buttonText.setPosition(textX, textY);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonText);
}

bool Button::isClicked(sf::Vector2i mousePos) {
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}
