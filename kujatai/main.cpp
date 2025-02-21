#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Menu.h"
#include "TextBox.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "Hahahahha");
    Menu menu(window.getSize().x, window.getSize().y);

    // โหลดพื้นหลัง
    sf::Texture texture;
    if (!texture.loadFromFile("mon.png")) {
        std::cerr << "Error loading Rush.png\n";
    }
    sf::Sprite background;
    background.setTexture(texture);

    // โหลด Tutorial (top)
    sf::Texture tot;
    if (!tot.loadFromFile("tuto.png")) {
        std::cerr << "Error loading tot.png\n";
    }
    sf::Sprite top;
    top.setTexture(tot);

    // หน้า play
    sf::Texture pl;
    if (!pl.loadFromFile("pl.JPG")) {
        std::cerr << "Error loading kuay.png\n";
    }
    sf::Sprite plays;
    plays.setTexture(pl);

    bool inTutorial = false; // โหมดแสดงหน้า Tutorial
    bool gameRunning = false; // เช็คสถานะการเล่นเกม
    bool isChoosingPlayers = false; // ตรวจสอบว่าอยู่ในโหมดเลือกจำนวนผู้เล่นหรือไม่
    bool isEnteringNames = false;  // ตัวแปรสำหรับการกรอกชื่อ
    bool isInPlayScreen = false; // ตัวแปรเพื่อเช็คว่าอยู่ในหน้า Play หรือไม่

    // ตัวเลือกจำนวนผู้เล่น
    int numberOfPlayers = 0; // ตัวแปรเก็บจำนวนผู้เล่นที่เลือก

    // สร้าง TextBox สำหรับกรอกชื่อผู้เล่น
    std::vector<TextBox> playerNameBoxes; // เก็บ TextBox สำหรับกรอกชื่อผู้เล่น
    std::vector<std::string> playerNames;  // เก็บชื่อผู้เล่น

    bool isNameEntered = false;  // ตัวแปรเช็คว่าผู้เล่นกรอกชื่อเสร็จแล้วหรือไม่

    // ฟังก์ชันนี้จะถูกเรียกเมื่อจำนวนผู้เล่นได้รับการเลือก
    auto createPlayerNameBoxes = [&](int numberOfPlayers, const sf::Font& font) {
        playerNameBoxes.clear();  // ล้าง TextBox เก่าก่อน

        float startY = 200;  // ตั้งตำแหน่งเริ่มต้นของ TextBox ให้ห่างกันพอสมควร
        for (int i = 0; i < numberOfPlayers; ++i) {
            // สร้าง TextBox ใหม่ให้กับผู้เล่น
            TextBox box(400, startY + i * 60, 200, 40);  // ปรับตำแหน่งตามความเหมาะสม
            box.setFont(font);
            playerNameBoxes.push_back(box);  // เพิ่มลงในเวกเตอร์
        }
        };

    // ฟังก์ชันเลือกจำนวนผู้เล่น
    auto showPlayerSelection = [&](const sf::Font& font) {
        std::vector<std::string> playerOptions = { "1 Player", "2 Players", "3 Players", "4 Players" };
        float startY = 150;
        for (int i = 0; i < playerOptions.size(); ++i) {
            sf::Text optionText(playerOptions[i], font, 30);
            optionText.setFillColor(sf::Color::Black);
            optionText.setPosition(400, startY + i * 60);
            window.draw(optionText);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (optionText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    numberOfPlayers = i + 1;  // กำหนดจำนวนผู้เล่นตามตัวเลือก
                    createPlayerNameBoxes(numberOfPlayers, font); // สร้าง TextBox ตามจำนวนผู้เล่น
                    isEnteringNames = true; // เปลี่ยนไปหน้าจอกรอกชื่อ
                    isChoosingPlayers = false; // เปลี่ยนโหมดไม่อยู่ในโหมดเลือกจำนวนผู้เล่นแล้ว
                    isInPlayScreen = true; // เปลี่ยนไปยังหน้าจอ Play
                }
            }
        }
        };

    // ฟังก์ชันสำหรับการกลับไปที่หน้าจอหลัก
    auto handleBackButton = [&]() {
        if (menu.IsBackClicked(window)) {
            if (isInPlayScreen) {
                isInPlayScreen = false; // กลับไปที่หน้าเลือกจำนวนผู้เล่น
                isEnteringNames = false; // ล้างข้อมูลกรอกชื่อ
                playerNameBoxes.clear();
            }
            else {
                // ถ้าอยู่ในหน้าเมนูหลัก
                isChoosingPlayers = false; // กลับไปที่เมนูหลัก
            }
        }
        };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                if (inTutorial) // ถ้าอยู่ในหน้าต่าง Tutorial
                {
                    if (menu.IsBackClicked(window)) {
                        inTutorial = false; // กลับไปที่เมนูหลัก
                    }
                }
                else if (gameRunning) // ถ้าอยู่เล่นเกม
                {
                    if (menu.IsBackClicked(window)) {
                        gameRunning = false; // กลับไปที่เมนูหลัก
                    }
                }
                else if (isChoosingPlayers) // ถ้าอยู่ในโหมดเลือกจำนวนผู้เล่น
                {
                    showPlayerSelection(menu.getFont()); // เรียกการเลือกจำนวนผู้เล่น
                }
                else // ถ้าอยู่ในเมนูหลัก
                {
                    for (int i = 0; i < menu.GetItemCount(); ++i)
                    {
                        if (menu.IsMouseOver(window, i)) // ตรวจสอบว่าคลิกที่ปุ่มไหน
                        {
                            switch (i)
                            {
                            case 0: // ปุ่ม Play
                                std::cout << "Play has been pressed" << std::endl;
                                isChoosingPlayers = true;  // เริ่มโหมดเลือกจำนวนผู้เล่น
                                break;
                            case 1: // ปุ่ม Tutorial
                                inTutorial = true; // ไปหน้า tutorial
                                break;
                            case 2: // ปุ่ม Exit
                                window.close(); // ปิด
                                break;
                            }
                        }
                    }
                }
            }
        }

        // ถ้าอยู่ในหน้า "กรอกชื่อ" ให้รับข้อมูลจาก TextBox
        if (isInPlayScreen) {
            for (auto& box : playerNameBoxes) {
                box.handleEvent(event);  // รับเหตุการณ์จาก TextBox ของผู้เล่น
            }

            // อัพเดตชื่อผู้เล่น
            playerNames.clear();
            for (auto& box : playerNameBoxes) {
                playerNames.push_back(box.getInput());  // รับข้อมูลชื่อจาก TextBox
            }
        }

        // วาด UI
        window.clear();
        if (inTutorial)
        {
            window.draw(top); // วาดหน้าจอ tutorial
            menu.drawBack(window); // แสดงปุ่ม Back
        }
        else if (gameRunning)
        {
            window.draw(plays); // วาดหน้าจอเกม
            menu.drawBack(window); // แสดงปุ่ม Back ถ้าต้องการ

            // แสดง TextBox ให้ผู้ใช้กรอกชื่อ
            for (auto& box : playerNameBoxes) {
                box.draw(window); // วาด TextBox
            }

            // แสดงข้อความชื่อผู้เล่น
            for (size_t i = 0; i < playerNames.size(); ++i) {
                if (!playerNames[i].empty()) {
                    sf::Text playerText;
                    playerText.setString("Player " + std::to_string(i + 1) + ": " + playerNames[i]);
                    playerText.setFont(menu.getFont());
                    playerText.setCharacterSize(30);
                    playerText.setFillColor(sf::Color::Black);
                    playerText.setPosition(200, 360 + i * 50);  // กำหนดตำแหน่งของข้อความ

                    window.draw(playerText);  // วาดข้อความในหน้าจอ
                }
            }
            // แสดงปุ่ม back ในหน้าเกม
            menu.drawBack(window);
        }
        else if (isInPlayScreen) {
            window.draw(plays); // วาดหน้าจอ Play (ใช้ pl.jpg)
            menu.drawBack(window); // วาดปุ่ม Back

            // แสดง TextBox ให้กรอกชื่อผู้เล่น
            for (auto& box : playerNameBoxes) {
                box.draw(window); // วาด TextBox
            }
        }
        else
        {
            window.draw(background); // เมนูหลัก
            menu.draw(window); // วาดเมนู

            // แสดงปุ่มเลือกจำนวนผู้เล่น
            if (isChoosingPlayers) {
                showPlayerSelection(menu.getFont()); // แสดงปุ่มเลือกจำนวนผู้เล่น
                menu.drawBack(window); // วาดปุ่ม Back
            }
        }

        window.display(); // แสดงผล
    }

    return 0;
}
