# SnakeGame
This is my implimentation of the game "Snake"

Background:
I've done similar projects in the past with C# and the .net framework but I wanted to use c++ for this project as thats what language I'm using in school. So I had to learn about win32, COM and Direct2D. Which turned out to be more difficult than I was expecting given that I was spoiled with .NET and the Visual Studio Designer. 

Project Desctiption:
Create a version of the game "Snake" that runs with win32 as well as make a pathfinding algorithm that the player can compete with. To do so I plan to make a launcher window that allows the player to select which gamemode they want to play as well as other options like board size. This launches the game window which controls the game logic.

Approach:
I plan to have snake window class which uses a control interface to run both the player controlled snake and the ai controlled snake. This means I will have virtual control interface with two seperate child classes that inherit from it. One for Computer Controlled snake and one for the Player. I also plan to have multiple objects for the Computer Player that use different pathfinding algorithms, I will allow the player to choose which version they want to compete against.