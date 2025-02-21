#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox
{
public:
    TextBox(float x, float y, float width, float height);
    void handleEvent(const sf::Event& event);  // ฟังก์ชันสำหรับจัดการกับเหตุการณ์การพิมพ์
    void draw(sf::RenderWindow& window);  // ฟังก์ชันสำหรับวาด TextBox
    void setFont(const sf::Font& font);  // ฟังก์ชันสำหรับตั้งฟอนต์
    std::string getInput() const;  // ฟังก์ชันสำหรับดึงข้อความที่กรอก
    void setInput(const std::string& text);  // ฟังก์ชันสำหรับกำหนดข้อความเริ่มต้น

private:
    sf::RectangleShape box;
    sf::Text text;
    std::string input;
    bool isActive;
};

#endif
