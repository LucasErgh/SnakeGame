#ifndef UNICODE
#define UNICODE
#endif 

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <Windows.h>

#include "TestView.h"
#include "Board.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return 0;
}

//// Test Mode in Command Line
//void TestMode();
//char getChar();
//
//int main()
//{
//    TestMode();
//
//    
//
//}
//
//// This runs the game in the command line
//void TestMode() {
//    int width = 20, height = 20;
//    TestView view = TestView();
//    bool alive = true;
//    Board board(width, height);
//    char input;
//    std::future<char> direction;
//    
//
//
//    direction = std::async(getChar);
//    do {
//
//        
//        if (direction.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
//            alive = board.doTurn();
//        else {
//            switch (direction.get())
//            {
//            case'w':
//                alive = board.doTurn(up);
//                break;
//            case'a':
//                alive = board.doTurn(left);
//                break;
//            case's':
//                alive = board.doTurn(down);
//                break;
//            case'd':
//                alive = board.doTurn(right);
//                break;
//            default:
//                break;
//            }
//            direction = std::async(getChar);
//        }
//
//        if(alive)
//            view.update(width, height, board.updateBoard());
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(500));
//
//    //    view.update(width, height, board.updateBoard());
//    //    std::cin >> input;
//
//    //    switch (input)
//    //    {
//    //    case'w':
//    //        alive = board.doTurn(up);
//    //        break;
//    //    case'a':
//    //        alive = board.doTurn(left);
//    //        break;
//    //    case's':
//    //        alive = board.doTurn(down);
//    //        break;
//    //    case'd':
//    //        alive = board.doTurn(right);
//    //        break;
//    //    default:
//    //        break;
//    //    }
//    } while (alive);
//    
//    std::cout << "You Died, hit any key then enter to continue" << std::endl;
//    direction.get();
//}
//
//// This is used for getting the new direction of the snake
//char getChar() {
//    char temp;
//    do {
//        temp = ' ';
//        std::cin >> temp;
//    } while (temp != 'a' && temp != 'w' && temp != 's' && temp != 'd');
//    return temp;
//}