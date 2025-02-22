#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "Menu.h"
#include "TextBox.h"
#include "Button.h"
#include "game.h" 

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

    bool inTutorial = false;
    bool isChoosingPlayers = false;
    bool isEnteringNames = false;
    bool isInStartScreen = false;

    int numberOfPlayers = 0;
    std::vector<TextBox> playerNameBoxes;
    std::vector<std::string> playerNames;
    std::vector<int> playerMoney; // เพิ่มตัวแปรเก็บเงินของผู้เล่น
    std::vector<std::string> playerMoneyChanges; //เก็บข้อความเงินที่ถูกสุ่ม

    sf::Font font;
    if (!font.loadFromFile("Rainbow Memories.otf")) {
        std::cerr << "Error loading font\n";
    }

    Button nextButton(450, 600, 150, 50, "Next", font);
    Button threePlayerButton(375, 300, 100, 50, "3", font);
    Button fourPlayerButton(500, 300, 100, 50, "4", font);
    Button fivePlayerButton(625, 300, 100, 50, "5", font);

    bool showNextButton = false;

    auto createPlayerNameBoxes = [&](int numberOfPlayers) {
        playerNameBoxes.clear();
        playerMoney.assign(numberOfPlayers, 50000); // เงินเริ่มต้น 
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
                    }
                }
                else if (isEnteringNames && nextButton.isClicked(mousePos)) {
                    playerNames.clear();
                    for (const auto& box : playerNameBoxes) {
                        playerNames.push_back(box.getText());
                    }

                    // สุ่มเงินและอัปเดตเงิน
                    randMoney(playerMoney, playerMoneyChanges);

                    isEnteringNames = false;
                    isInStartScreen = true;
                }
                else if (menu.IsBackClicked(window)) {
                    inTutorial = false;
                    isChoosingPlayers = false;
                    isEnteringNames = false;
                    isInStartScreen = false;
                    menu.show();
                }
                else if (!(isChoosingPlayers || isEnteringNames || inTutorial || isInStartScreen)) {
                    for (int i = 0; i < menu.GetItemCount(); ++i) {
                        if (menu.IsMouseOver(window, i)) {
                            switch (i)
                            {
                            case 0:
                                isChoosingPlayers = true;
                                menu.hide();
                                break;
                            case 1:
                                inTutorial = true;
                                menu.hide();
                                break;
                            case 2:
                                window.close();
                                break;
                            }
                        }
                    }
                }
            }

            if (isEnteringNames) {
                for (auto& box : playerNameBoxes) {
                    box.handleEvent(event);
                }
            }
        }

        window.clear();
        if (inTutorial) {
            window.draw(top);
        }
        else if (isChoosingPlayers) {
            window.draw(kornplays);
            threePlayerButton.draw(window);
            fourPlayerButton.draw(window);
            fivePlayerButton.draw(window);
        }
        else if (isEnteringNames) {
            window.draw(kornplays);
            for (auto& box : playerNameBoxes) {
                box.draw(window);
            }
            nextButton.draw(window);
        }
        else if (isInStartScreen) {
            window.draw(startja);

            float startY = 10;
            float startX = 10;

            //  แสดงชื่อและเงิน
            for (size_t i = 0; i < playerMoneyChanges.size(); ++i) {
                sf::Text playerText;
                playerText.setFont(font);
                playerText.setString(playerNames[i] + " - " + playerMoneyChanges[i]);
                playerText.setCharacterSize(40);
                playerText.setFillColor(sf::Color::White);
                playerText.setPosition(startX, startY + i * 50);
                window.draw(playerText);
            }
        }
        else {
            window.draw(background);
            menu.draw(window);
        }

        window.display();
    }

    return 0;
}
