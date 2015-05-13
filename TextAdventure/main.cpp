#include <iostream>

#include "Dungeon.h"
#include "Player.h"

void main()
{
	Dungeon::Instance().SetDungeonDimensions();
	Dungeon::Instance().FillRooms();
	
	CustomString playerName;

	while (playerName == " " || playerName.IsEmpty())
	{
		playerName.ReadFromConsole("Welcome, please enter a name for the character.");
	}

	Player player = Player(playerName.c_str(), Dungeon::Instance().GetStartingRoom()->GetID());

	if (Dungeon::Instance().GetStartingRoom() == nullptr)
	{
		return;
	}

	playerName = CustomString("Welcome ") + playerName;
	playerName.WriteToConsole();

	Room* currentRoom = nullptr;

	while (!Dungeon::Instance().GetDungeonFinished())
	{
		printf("Current room is %i\n", player.GetCurrentPosition());

		currentRoom = Dungeon::Instance().GetRoom(player.GetCurrentPosition());

		if (currentRoom != nullptr)
		{
			currentRoom->PrintDescription();
			currentRoom->ProcessItemSelection(&player);
			currentRoom->ProcessInput(&player);
		}
	}

	printf("Goodbye\n");

	system("pause");
}