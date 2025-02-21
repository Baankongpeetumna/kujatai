#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SFML/Graphics.hpp>

#define MAX_ITEM 3  

class Menu {
public:
    Menu(float width, float height);
    ~Menu();

    void draw(sf::RenderWindow& window);  // วาดเมนูหลัก
    void drawBack(sf::RenderWindow& window); // วาดปุ่ม Back
    void MoveUp();
    void MoveDown();
    int GetPressedItem() { return selectedItem; }
    bool IsMouseOver(sf::RenderWindow& window, int index);
    bool IsBackClicked(sf::RenderWindow& window); // เช็คว่าคลิกปุ่ม Back หรือไม่
    int GetItemCount();

    // ฟังก์ชันนี้จะคืนค่า reference ของ font
    sf::Font& getFont() { return font; }

private:
    int selectedItem = 0;
    sf::Font font;  // ฟอนต์ที่ใช้ในเมนู
    sf::Text menu[MAX_ITEM];  // ตัวเลือกในเมนูหลัก
    sf::Text backButton; // ปุ่ม Back สำหรับหน้า Tutorial
};

#endif
