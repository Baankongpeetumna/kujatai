#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox
{
public:
    TextBox(float x, float y, float width, float height);
    void handleEvent(const sf::Event& event);  // จัดการเหตุการณ์พิมพ์
    void draw(sf::RenderWindow& window);  // วาด TextBox
    void setFont(const sf::Font& font);  // ตั้งฟอนต์
    std::string getInput() const;  // ดึงข้อความที่กรอก
    void setInput(const std::string& text);  // กำหนดข้อความเริ่มต้น
    std::string getText() const { return text.getString().toAnsiString(); } // ดึงข้อความ

private:
    sf::RectangleShape box;
    sf::Text text;
    std::string input;  // ใช้เก็บข้อความที่พิมพ์
    bool isActive;
};

#endif
