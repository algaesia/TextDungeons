#include "RoomTypes.h"
#include "RandNum.h"
#include "Dungeon.h"
#include <string>

EmptyRoom::EmptyRoom()
	: Room("This is a boring empty room.",
		   "Another plain, boring empty room.")
{

}

void EmptyRoom::ProcessInput(Player* a_Player)
{
	visited = true;
	Room::ProcessInput(a_Player);
}

RiddleRoom::RiddleRoom()
	: Room("A riddle protects the treasure in this room.")
{

}

void RiddleRoom::ProcessInput(Player* a_Player)
{
	if (!defeated)
	{
		m_RoomQuestion.ReadFromConsole("What loses its head in the morning and gets it back at night?");

		if (m_RoomQuestion == "pillow" ||
			m_RoomQuestion == "a pillow")
		{
			defeated = true;
			Room::ProcessInput(a_Player);
		}
		else
		{
			m_RoomQuestion = "This is incorrect, try again";
			m_RoomQuestion.WriteToConsole();
			m_RoomQuestion = " ";
		}
	}
	else
	{
		Room::ProcessInput(a_Player);
	}
}

BossRoom::BossRoom()
	: Room("A gargantuan beast looms menacingly over its domain."),
	chosenNum(0),
	strings()
{
	chosenNum = (int)(RandNum::Instance().GetRandNum() * 7);
	strings[0] = "pride";
	strings[1] = "gluttony";
	strings[2] = "wrath";
	strings[3] = "lust";
	strings[4] = "greed";
	strings[5] = "envy";
	strings[6] = "sloth";
}

void BossRoom::ProcessInput(Player* a_Player)
{
	if (!defeated)
	{
		visited = true;
		m_RoomQuestion.ReadFromConsole("There are seven deadly sins. One of them is this beast's flaw.");

		if (m_RoomQuestion == strings[chosenNum])
		{
			m_RoomQuestion = "Shocked at the cunning perception, the beast slinks backward into the darkness, removing any further obstacles.";
			m_RoomQuestion.WriteToConsole();
			m_RoomQuestion = " ";

			defeated = true;
			Room::ProcessInput(a_Player);
		}
		else
		{
			m_RoomQuestion = "The beast takes swing, indignant that you would accuse it of such things.";
			m_RoomQuestion.WriteToConsole();
			m_RoomQuestion = " ";
		}
	}
	else
	{
		Room::ProcessInput(a_Player);
	}
}

TrapRoom::TrapRoom()
	: Room("Someone triggered the trap, plunging the room into darkness.")
{

}

void TrapRoom::ProcessInput(Player* a_Player)
{
	m_RoomQuestion = "Upon waking up, the surroundings are new.";
	m_RoomQuestion.WriteToConsole();
	m_RoomQuestion = " ";

	int currentPos = a_Player->GetCurrentPosition();
	int newPosCounter = 0;
	int maxCounter = 5;

	while (newPosCounter < maxCounter)
	{
		Room* newRoom = Dungeon::Instance().GetValidRandomRoom();

		if (newRoom != nullptr)
		{
			int newPos = newRoom->GetID();
			if (currentPos != newPos)
			{
				a_Player->SetCurrentPosition(newPos);
			}
		}

		newPosCounter++;
	}

	Room::ProcessInput(a_Player);
}

TreasureRoom::TreasureRoom()
	: Room("Sweet treasure is the highlight of this room.",
		   "So much treasure, and not a guard in sight.")
{

}

void TreasureRoom::ProcessInput(Player* a_Player)
{
	visited = true;
	Room::ProcessInput(a_Player);
}

MonsterRoom::MonsterRoom()
	: Room("Hostile creatures on all sides, trapped, behind bars.",
	       "The creatures are still there, reaching out for an unassuming passer-by.")
{

}

void MonsterRoom::ProcessInput(Player* a_Player)
{
	visited = true;
	Room::ProcessInput(a_Player);
}

VistaRoom::VistaRoom()
	: Room("A beautiful vista opens up, and on the other side the edge falls down into a huge void.",
		   "Above the horizon, a panorama of the creamy blue sky, lined with puffy clouds, below, a deep gorge, with no ground in sight.")
{

}

void VistaRoom::ProcessInput(Player* a_Player)
{
	m_RoomQuestion.ReadFromConsole("You stand there, in solitude, staring into the distance. Where to next?");

	visited = true;

	if (m_RoomQuestion == "jump")
	{
		m_RoomQuestion = "You take a leap of faith, into the abyss, clouds swallowing you up...\n\n\n\n\nYou wake up somewhere strangely familiar.";
		m_RoomQuestion.WriteToConsole();
		Dungeon::Instance().ResetDungeon();
		a_Player->SetCurrentPosition(Dungeon::Instance().GetStartingRoom()->GetID());
	}
	else
	{
		m_RoomQuestion = "You decide, instead, to move away from the ledge and continue on with the dungeon at hand.";
		m_RoomQuestion.WriteToConsole();

		Room::ProcessInput(a_Player);
	}
}

EntranceRoom::EntranceRoom()
	: Room("The beginning of the adventure. So many opportunities await in the dungeon ahead.",
		   "Back at the entrance, are we going around in circles?")
{

}

void EntranceRoom::ProcessInput(Player* a_Player)
{
	visited = true;
	Room::ProcessInput(a_Player);
}

ExitRoom::ExitRoom()
	: Room("The end of the adventure. Opportunities extinguished, the adventurers leave the dungeon.",
		   "Should we leave this time round?")
{

}

void ExitRoom::ProcessInput(Player* a_Player)
{
	m_RoomQuestion.ReadFromConsole("The dungeon has come to an end. Would you like to start over?");

	if (m_RoomQuestion == "no")
	{
		//quit game
		Dungeon::Instance().SetDungeonFinished(true);

		int score = a_Player->GetTotalScore();
		CustomString finalScore = CustomString("Final score is: ") + CustomString(std::to_string(score).c_str());
		finalScore.WriteToConsole();
		return;
	}
	else if (m_RoomQuestion == "yes")
	{
		//restart dungeon here
		Dungeon::Instance().ResetDungeon();
		a_Player->SetCurrentPosition(Dungeon::Instance().GetStartingRoom()->GetID());
	}
	else
	{
		//don't do anything, player can keep wandering around dungeon
		Room::ProcessInput(a_Player);
		visited = true;
	}
}

DeadEndRoom::DeadEndRoom()
	: Room("It's a dead end, no way to progress further, need to turn back.",
	       "Another dead end.")
{

}

void DeadEndRoom::ProcessInput(Player* a_Player)
{
	m_RoomQuestion.ReadFromConsole("You stare at the enclosed walls, annoyed at the dead-endedness, how will you express your frustration?");

	visited = true;

	if (m_RoomQuestion == "punch" || m_RoomQuestion == "kick" || m_RoomQuestion == "punch the wall" || m_RoomQuestion == "kick the wall")
	{
		m_RoomQuestion = "You've uncovered the secret exit, making short work of the dungeon and returning hastily to the surface.";
		m_RoomQuestion.WriteToConsole();

		Dungeon::Instance().SetDungeonFinished(true);
	}
	else
	{
		m_RoomQuestion = "You failed to make any impressionable difference to the walls.";
		m_RoomQuestion.WriteToConsole();

		Room::ProcessInput(a_Player);
	}
}

NextLevelRoom::NextLevelRoom()
	: Room("A hole in the ground",
	       "Take a leap of faith into the unknown.")
{

}

void NextLevelRoom::ProcessInput(Player* a_Player)
{
	m_RoomQuestion.ReadFromConsole("This leads to a new area of the dungeon, where exactly it leads is unclear. Proceed further?");

	if (m_RoomQuestion == "no")
	{
		//don't do anything, player can keep wandering around dungeon
		Room::ProcessInput(a_Player);
		visited = true;
	}
	else
	{
		//restart dungeon here
		Dungeon::Instance().ResetDungeon();
		a_Player->SetCurrentPosition(Dungeon::Instance().GetStartingRoom()->GetID());
	}
}
