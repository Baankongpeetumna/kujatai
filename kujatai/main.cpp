#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp> 
#include <iostream> 
#include <vector> 
#include "Menu.h" 
#include "TextBox.h" 
#include "Button.h" 
#include "game.h" 
#include "gP_2.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "Hahahahha");
    Menu menu(window.getSize().x, window.getSize().y);

    sf::Texture texture;
    if (!texture.loadFromFile("mon.png")) {
        std::cerr << "Error loading mon.png\n";
    }
    sf::Sprite background(texture);

    sf::Texture tot;
    if (!tot.loadFromFile("tuto.png")) {
        std::cerr << "Error loading tuto.png\n";
    }
    sf::Sprite top(tot);

    sf::Texture kornpl;
    if (!kornpl.loadFromFile("BGja.JPEG")) {
        std::cerr << "Error loading BGja.JPEG\n";
    }
    sf::Sprite kornplays(kornpl);

    sf::Texture start;
    if (!start.loadFromFile("startska.JPG")) {
        std::cerr << "Error loading startska.JPG\n";
    }
    sf::Sprite startja(start);

    StockGraph stockGraph(800, 400);
    sf::RectangleShape graphArea(sf::Vector2f(800, 400));
    graphArea.setPosition(140, 150);
    graphArea.setFillColor(sf::Color::Black);

    sf::Texture choice;
    if (!choice.loadFromFile("choice.JPG")) {
        std::cerr << "Error loading choice.JPG\n";
    }
    sf::Sprite choicenaka(choice);

    bool inTutorial = false;
    bool isChoosingPlayers = false;
    bool isEnteringNames = false;
    bool isInStartScreen = false;
    bool isInGraphScreen = false;
    bool isInChoiceScreen = false; // เพิ่มสถานะสำหรับหน้าของ choice

    bool menuVisible = true;  // เมนูจะแสดงเมื่อเริ่มต้น

    int numberOfPlayers = 0;
    std::vector<TextBox> playerNameBoxes;
    std::vector<std::string> playerNames;
    std::vector<int> playerMoney;
    std::vector<std::string> playerMoneyChanges;

    sf::Font font;
    if (!font.loadFromFile("Rainbow Memories.otf")) {
        std::cerr << "Error loading font\n";
    }

    Button nextButton(450, 600, 150, 50, "Next", font);
    Button graphButton(450, 650, 150, 50, "Show Graph", font);
    Button proceedButton(450, 700, 150, 50, "Proceed", font);
    Button backButton(10, 10, 100, 40, "Back", font);
    Button threePlayerButton(375, 300, 100, 50, "3", font);
    Button fourPlayerButton(500, 300, 100, 50, "4", font);
    Button fivePlayerButton(625, 300, 100, 50, "5", font);

    bool showNextButton = false;

    auto createPlayerNameBoxes = [&](int numberOfPlayers) {
        playerNameBoxes.clear();
        playerMoney.assign(numberOfPlayers, 50000);
        float startY = 200;
        for (int i = 0; i < numberOfPlayers; ++i) {
            TextBox box(400, startY + i * 60, 200, 40);
            box.setFont(font);
            playerNameBoxes.push_back(box);
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
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (isChoosingPlayers) {
                    if (threePlayerButton.isClicked(mousePos)) numberOfPlayers = 3;
                    else if (fourPlayerButton.isClicked(mousePos)) numberOfPlayers = 4;
                    else if (fivePlayerButton.isClicked(mousePos)) numberOfPlayers = 5;

                    if (numberOfPlayers > 0) {
                        createPlayerNameBoxes(numberOfPlayers);
                        isChoosingPlayers = false;
                        isEnteringNames = true;
                        menuVisible = false;  // ซ่อนเมนูเมื่อเข้าสู่การกรอกชื่อ
                    }
                    if (backButton.isClicked(mousePos)) {
                        isChoosingPlayers = false;
                        menu.show();
                        menuVisible = true;  // แสดงเมนูเมื่อกลับไปยังหน้าเมนูหลัก
                    }
                }
                else if (isEnteringNames) {
                    if (nextButton.isClicked(mousePos)) {
                        playerNames.clear();
                        for (const auto& box : playerNameBoxes) {
                            playerNames.push_back(box.getText());
                        }
                        randMoney(playerMoney, playerMoneyChanges);
                        isEnteringNames = false;
                        isInStartScreen = true;
                        menuVisible = false;  // ซ่อนเมนูเมื่อเริ่มเกม
                    }
                    else if (backButton.isClicked(mousePos)) {
                        isEnteringNames = false;
                        isChoosingPlayers = true; // กลับไปหน้าเลือกจำนวนผู้เล่น
                        menuVisible = true;  // แสดงเมนูเมื่อกลับไปยังหน้าเลือกผู้เล่น
                    }
                }
                else if (menu.IsBackClicked(window)) {
                    inTutorial = false;
                    isChoosingPlayers = false;
                    isEnteringNames = false;
                    isInStartScreen = false;
                    menu.show();  // แสดงเมนูหลัก
                    menuVisible = true;  // แสดงเมนูเมื่อกลับไปหน้าเมนูหลัก
                }
                else if (!(isChoosingPlayers || isEnteringNames || inTutorial || isInStartScreen)) {
                    for (int i = 0; i < menu.GetItemCount(); ++i) {
                        if (menu.IsMouseOver(window, i)) {
                            switch (i)
                            {
                            case 0:
                                isChoosingPlayers = true;
                                menu.hide();
                                menuVisible = false;  // ซ่อนเมนูเมื่อเข้าสู่การเลือกผู้เล่น
                                break;
                            case 1:
                                inTutorial = true;
                                menu.hide();
                                menuVisible = false;  // ซ่อนเมนูเมื่อเข้าสู่การสอน
                                break;
                            case 2:
                                window.close();
                                break;
                            }
                        }
                    }
                }

                // เพิ่มการตรวจสอบคลิกที่ปุ่ม graphButton
                if (isInStartScreen && graphButton.isClicked(mousePos)) {
                    isInGraphScreen = true;
                    isInStartScreen = false; // เปลี่ยนไปที่หน้ากราฟ
                    menuVisible = false;  // ซ่อนเมนูเมื่ออยู่ในหน้ากราฟ
                }

                // ตรวจสอบการคลิกที่ปุ่ม "Next" ในหน้ากราฟ
                if (isInGraphScreen && nextButton.isClicked(mousePos)) {
                    // เมื่อกดปุ่ม "Next" เปลี่ยนไปที่หน้า choicenaka
                    isInGraphScreen = false; // ออกจากหน้ากราฟ
                    isInChoiceScreen = true; // เปลี่ยนไปยังหน้าของ choice
                    menuVisible = false;  // ซ่อนเมนูเมื่อไปที่หน้าการเลือก
                }
            }

            if (isEnteringNames) {
                for (auto& box : playerNameBoxes) {
                    box.handleEvent(event);
                }
            }
        }

        window.clear();
        if (menuVisible) {
            window.draw(background);
            menu.draw(window);  // แสดงเมนูถ้า menuVisible เป็น true
        }
        else if (inTutorial) {
            window.draw(top);
        }
        else if (isChoosingPlayers) {
            window.draw(kornplays);
            threePlayerButton.draw(window);
            fourPlayerButton.draw(window);
            fivePlayerButton.draw(window);
            backButton.draw(window);
        }
        else if (isEnteringNames) {
            window.draw(kornplays);
            for (auto& box : playerNameBoxes) {
                box.draw(window);
            }
            nextButton.draw(window);
            backButton.draw(window);
        }
        else if (isInStartScreen) {
            window.draw(startja);
            float startY = 10;
            float startX = 10;

            // แสดงชื่อและเงิน
            for (size_t i = 0; i < playerMoneyChanges.size(); ++i) {
                sf::Text playerText;
                playerText.setFont(font);
                playerText.setString(playerNames[i] + " - " + playerMoneyChanges[i]);
                playerText.setCharacterSize(40);
                playerText.setFillColor(sf::Color::White);
                playerText.setPosition(startX, startY + i * 50);
                window.draw(playerText);
            }
            graphButton.draw(window);
        }
        else if (isInGraphScreen) {
            window.draw(graphArea);
            stockGraph.drawGraph(window, graphArea);
            nextButton.draw(window); // วาดปุ่ม "Next" ในหน้าจอกราฟ
        }
        else if (isInChoiceScreen) {
            window.draw(choicenaka); // วาดหน้า choicenaka เมื่อไปยังหน้าของ choice
        }
        window.display();
    }
    return 0;
}
