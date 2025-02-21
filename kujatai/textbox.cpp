#include "TextBox.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

TextBox::TextBox(float x, float y, float width, float height)
    : isActive(false), input("")
{
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2);
    box.setPosition(x, y);
}

void TextBox::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        // ตรวจสอบว่าเกิดการคลิกในช่อง TextBox หรือไม่
        if (box.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
        {
            isActive = true;  // เปิดใช้งาน TextBox
        }
        else
        {
            isActive = false;  // ปิดใช้งาน TextBox
        }
    }

    if (isActive && event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == 8 && input.length() > 0) // 8 = backspace
        {
            input.pop_back(); // ลบอักขระสุดท้าย
        }
        else if (event.text.unicode < 128) // ถ้าเป็นอักขระที่สามารถพิมพ์ได้
        {
            input += static_cast<char>(event.text.unicode); // เพิ่มอักขระใหม่
        }
    }
}

void TextBox::draw(sf::RenderWindow& window)
{
    window.draw(box);  // วาดกรอบ TextBox

    text.setString(input);  // กำหนดข้อความที่จะแสดงใน TextBox
    text.setPosition(box.getPosition().x + 5, box.getPosition().y + 5);  // ตั้งตำแหน่งข้อความ
    window.draw(text);  // วาดข้อความที่กรอก
}

void TextBox::setFont(const sf::Font& font)
{
    text.setFont(font);  // ตั้งฟอนต์
    text.setCharacterSize(24);  // ตั้งขนาดตัวอักษร
    text.setFillColor(sf::Color::Black);  // ตั้งสีตัวอักษร
}

std::string TextBox::getInput() const
{
    return input;  // คืนค่าข้อความที่กรอกใน TextBox
}

void TextBox::setInput(const std::string& text)
{
    input = text;  // กำหนดข้อความเริ่มต้น
}
