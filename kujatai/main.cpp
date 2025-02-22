#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "Menu.h"
#include "TextBox.h"
#include "Button.h"
#include "game.h"
#include "gP_2.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "Stock Trading Game");

    Menu menu(window.getSize().x, window.getSize().y);

    // Load background images
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

    // Create graph for StockGraph
    StockGraph stockGraph(800, 400);
    sf::RectangleShape graphArea(sf::Vector2f(800, 400));
    graphArea.setPosition(140, 150);
    graphArea.setFillColor(sf::Color::Black);

    // Choice menu texture
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
    bool isInChoiceScreen = false;  // New state for choice screen

    bool menuVisible = true;  // Show menu initially

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

    Button buySharesButton(375, 500, 150, 50, "Buy Shares", font);
    Button sellSharesButton(375, 570, 150, 50, "Sell Shares", font);
    Button skipTurnButton(375, 640, 150, 50, "Skip Turn", font);

    // TextBox for number of shares input
    TextBox buySharesTextBox(550, 500, 100, 40);
    TextBox sellSharesTextBox(550, 570, 100, 40);
    bool showBuySharesTextBox = false;
    bool showSellSharesTextBox = false;

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

    int currentPlayer = 0;  // Add a variable to track the current player
    int turnCounter = 0;  // Track how many turns have been taken
    int roundCounter = 0; // Track the number of rounds (up to 5 rounds)

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (isChoosingPlayers) {
                    if (threePlayerButton.isClicked(mousePos)) numberOfPlayers = 3;
                    else if (fourPlayerButton.isClicked(mousePos)) numberOfPlayers = 4;
                    else if (fivePlayerButton.isClicked(mousePos)) numberOfPlayers = 5;

                    if (numberOfPlayers > 0) {
                        createPlayerNameBoxes(numberOfPlayers);
                        isChoosingPlayers = false;
                        isEnteringNames = true;
                        menuVisible = false;  // Hide menu when entering names
                    }

                    if (backButton.isClicked(mousePos)) {
                        isChoosingPlayers = false;
                        menu.show();
                        menuVisible = true;  // Show menu when going back to the main menu
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
                        menuVisible = false;  // Hide menu when starting game
                    }
                    else if (backButton.isClicked(mousePos)) {
                        isEnteringNames = false;
                        isChoosingPlayers = true; // Go back to player selection screen
                        menuVisible = true;  // Show menu when going back to player selection
                    }
                }
                else if (menu.IsBackClicked(window)) {
                    inTutorial = false;
                    isChoosingPlayers = false;
                    isEnteringNames = false;
                    isInStartScreen = false;
                    menu.show();  // Show main menu
                    menuVisible = true;  // Show menu when going back to the main menu
                }
                else if (!(isChoosingPlayers || isEnteringNames || inTutorial || isInStartScreen)) {
                    for (int i = 0; i < menu.GetItemCount(); ++i) {
                        if (menu.IsMouseOver(window, i)) {
                            switch (i) {
                            case 0:
                                isChoosingPlayers = true;
                                menu.hide();
                                menuVisible = false;  // Hide menu when going to player selection
                                break;
                            case 1:
                                inTutorial = true;
                                menu.hide();
                                menuVisible = false;  // Hide menu when going to tutorial
                                break;
                            case 2:
                                window.close();
                                break;
                            }
                        }
                    }
                }

                if (isInStartScreen && graphButton.isClicked(mousePos)) {
                    isInGraphScreen = true;
                    isInStartScreen = false; // Go to graph screen
                    menuVisible = false;  // Hide menu when in graph screen
                }

                if (isInGraphScreen && nextButton.isClicked(mousePos)) {
                    isInGraphScreen = false; // Exit graph screen
                    isInChoiceScreen = true; // Go to choice screen
                    menuVisible = false;  // Hide menu when going to choice screen
                }

                // Handle player choices for Buy, Sell, or Skip
                if (isInChoiceScreen) {
                    if (buySharesButton.isClicked(mousePos)) {
                        showBuySharesTextBox = true;  // Show the text box for input
                        showSellSharesTextBox = false; // Hide the sell input
                    }
                    else if (sellSharesButton.isClicked(mousePos)) {
                        showSellSharesTextBox = true;  // Show the text box for input
                        showBuySharesTextBox = false; // Hide the buy input
                    }
                    else if (skipTurnButton.isClicked(mousePos)) {
                        // Skip turn and go to the next player
                        currentPlayer = (currentPlayer + 1) % numberOfPlayers;  // Move to the next player
                        turnCounter++;

                        if (turnCounter == numberOfPlayers) {
                            // All players have played, reset turn counter or end the round
                            std::cout << "All players have finished their turns.\n";
                            // Refresh the graph for the next round
                            stockGraph.refreshGraph(); // Refresh the graph
                            turnCounter = 0;
                            roundCounter++; // Increment the round

                            if (roundCounter == 5) {
                                // If we've completed 5 rounds, end the game or show final results
                                std::cout << "5 rounds completed.\n";
                                // Reset game if needed, or end the game
                                roundCounter = 0; // Reset round counter
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

            // Handle TextBox events
            if (showBuySharesTextBox) {
                buySharesTextBox.handleEvent(event);
            }
            if (showSellSharesTextBox) {
                sellSharesTextBox.handleEvent(event);
            }
        }

        window.clear();
        if (menuVisible) {
            window.draw(background);
            menu.draw(window);  // Draw menu if menuVisible is true
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

            // Show player names and money changes
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
            nextButton.draw(window); // Draw "Next" button in graph screen
        }
        else if (isInChoiceScreen) {
            window.draw(choicenaka); // Draw choice screen
            buySharesButton.draw(window);
            sellSharesButton.draw(window);
            skipTurnButton.draw(window);

            // Draw the text boxes if needed
            if (showBuySharesTextBox) {
                buySharesTextBox.draw(window);
            }
            if (showSellSharesTextBox) {
                sellSharesTextBox.draw(window);
            }

            // Display current player's turn above the Skip Turn button
            sf::Text turnText;
            turnText.setFont(font);
            turnText.setString("Turn of: " + playerNames[currentPlayer]);
            turnText.setCharacterSize(30);
            turnText.setFillColor(sf::Color::White);
            turnText.setPosition(375, 590); // Adjust position above the button
            window.draw(turnText);
        }
        window.display();
    }
    return 0;
}
