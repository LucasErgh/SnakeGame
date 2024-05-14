#include <iostream>
#include "View.h"
#include "Board.h"

void TestMode();

int main()
{
    int mode;
    std::cout << "Welcome to Snake!" << std::endl 
        << "Select your mode: " << std::endl
        << "1. Test Mode" << std::endl
        << "2. Command Line Mode" << std::endl
        << "3. Windows Mode" << std::endl
        << "4. Computer Controlled" << std::endl;
    do {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cin >> mode;
    } while (std::cin.fail() || mode > 4 || mode < 1);

    View view;
    switch (mode)
    {
    case 1:
        view = new TestView();
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }

}

void TestMode() {

}