#include <iostream>
#include "ui.h"

int main() {
    int choice;
    while (true) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: functionalTests(); break;
            case 2: benchmarkMenu(); break;
            case 3: demo(); break;
            case 4: showMemorySizes(); break;
            case 0: return 0;
            default: std::cout << "Wrong choice\n";
        }
    }
    return 0;
}