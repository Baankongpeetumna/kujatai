#include "Menu.h"
#include <iostream> 

#define MAX_ITEM 3  

Menu::Menu(float width, float height)
{
    if (!font.loadFromFile("Rainbow Memories.otf")) {
        std::cerr << "Error loading font!\n";
    }

    selectedItem = 0; // ค่าเริ่มต้นของเมนูที่ถูกเลือก

    // ตั้งค่าปุ่มเมนูหลัก
    menu[0].setFont(font);
    menu[0].setCharacterSize(85);
    menu[0].setFillColor(sf::Color::White);
    menu[0].setString("Play");
    menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_ITEM + 1) * 1));
    menu[0].setOrigin(menu[0].getGlobalBounds().width / 2, menu[0].getGlobalBounds().height / 2);

    menu[1].setFont(font);
    menu[1].setCharacterSize(80);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setString("Tutorial");
    menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_ITEM + 1) * 2));
    menu[1].setOrigin(menu[1].getGlobalBounds().width / 2, menu[1].getGlobalBounds().height / 2);

    menu[2].setFont(font);
    menu[2].setCharacterSize(80);
    menu[2].setFillColor(sf::Color::White);
    menu[2].setString("Exit");
    menu[2].setPosition(sf::Vector2f(width / 2, height / (MAX_ITEM + 1) * 3));
    menu[2].setOrigin(menu[2].getGlobalBounds().width / 2, menu[2].getGlobalBounds().height / 2);

    // ตั้งค่าปุ่ม Back
    backButton.setFont(font);
    backButton.setCharacterSize(55);
    backButton.setFillColor(sf::Color::White);
    backButton.setString("Back");
    backButton.setPosition(70, 20);
}

Menu::~Menu() {}

void Menu::draw(sf::RenderWindow& window) {
    for (int i = 0; i < MAX_ITEM; i++) {
        window.draw(menu[i]);
    }
}

void Menu::drawBack(sf::RenderWindow& window) {
    window.draw(backButton);
}

void Menu::MoveUp()
{
    if (selectedItem - 1 >= 0)
    {
        menu[selectedItem].setFillColor(sf::Color::White);
        selectedItem--;
        menu[selectedItem].setFillColor(sf::Color::Red);
    }
}

void Menu::MoveDown()
{
    if (selectedItem + 1 < MAX_ITEM)
    {
        menu[selectedItem].setFillColor(sf::Color::White);
        selectedItem++;
        menu[selectedItem].setFillColor(sf::Color::Red);
    }
}

bool Menu::IsMouseOver(sf::RenderWindow& window, int index)
{
    if (index < 0 || index >= MAX_ITEM) return false; // ป้องกัน index ผิดพลาด

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect buttonBounds = menu[index].getGlobalBounds();

    return buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

bool Menu::IsBackClicked(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

int Menu::GetItemCount() {
    return MAX_ITEM;
}
