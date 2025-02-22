#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void randMoney(std::vector<int>& playerMoney, std::vector<std::string>& playerMoneyChanges) {
    static bool isSeedSet = false;
    if (!isSeedSet) {
        srand(time(0));
        isSeedSet = true;
    }

    int amounts[] = { 1000, 2000, -1000, -2000, 5000, 3000, 4000, -2000, 4600, -3000, 5600, 4000, -3440, 1000 };
    int numAmounts = sizeof(amounts) / sizeof(amounts[0]);

    playerMoneyChanges.clear(); // ??????????????????????????????????

    for (size_t i = 0; i < playerMoney.size(); i++) {
        int change = amounts[rand() % numAmounts];
        playerMoney[i] += change;

        // ?????????????????????? std::cout
        playerMoneyChanges.push_back(
            "Player " + std::to_string(i + 1) +
            (change >= 0 ? " received: +" : " lost: ") +
            std::to_string(change) + " dollars | Balance: " +
            std::to_string(playerMoney[i]) + " dollars"
        );
    }
}
