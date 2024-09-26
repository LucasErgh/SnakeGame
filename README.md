# SnakeGame
This is my implementation of the game "Snake"

## Background:
I've done similar projects in the past with C# and the .NET framework but I wanted to use C++ for this project as I'm starting to learn C++ for school. Further, it was also something that would allow me to learn about Win32, COM and Direct2D.

## Project Description:
Develop a Win32 based version of the game "Snake" with a pathfinding algorithm for the player to compete with. The project includes three game modes, single player, computer player, and player vs computer. The pathfinding uses an altered version of the A* pathfinding algorithm and an algorithm to create a Hamilton cycle with maximum turns on a square grid to safely get apples.

## PROBLEMS:
Since I started this project I've learned a lot, my code has multiple areas for improvement. This includes things like consistent naming, code duplication, and unused C++ features. I could have been using smart pointers, nullptr instead of NULL, list or deque instead of writing my own linked list to represent the snake (I chose to do that for fun), const where in functions were applicable, and using destructors instead of resources freeing functions. Moreover, there are improvements to the pathfinding function I could make. For example, checking if the path is safe in the A* logic rather than running A* then checking if it was safe.
