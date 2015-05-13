#include "Room.h"
#include <iostream>
#include "RandNum.h"
#include "Dungeon.h"

int Room::ID = 0;

Room::Room(const CustomString& a_RoomDescription, const CustomString& a_RoomAltDescription)
	: m_RoomDescription(a_RoomDescription),
	m_RoomAltDescription(a_RoomAltDescription),
	m_RoomQuestion(),
	m_MaxItems(5),
	north(nullptr), south(nullptr), east(nullptr), west(nullptr),
	m_ID(0), visited(false), defeated(false)
{
	m_ID = ID;
	ID++;

	GenerateItems();
}

int Room::NumConnections() const
{
	int counter = 0;

	if (north != nullptr)
		counter++;

	if (south != nullptr)
		counter++;

	if (east != nullptr)
		counter++;

	if (west != nullptr)
		counter++;

	return counter;
}

bool Room::FullyConnected() const
{
	//corners
	if (m_ID == 0 ||
		m_ID == Dungeon::Instance().GetDungeonWidth() - 1 ||
		m_ID == Dungeon::Instance().GetDungeonWidth() * Dungeon::Instance().GetDungeonHeight() - 1 ||
		m_ID == Dungeon::Instance().GetDungeonWidth() * (Dungeon::Instance().GetDungeonHeight() - 1))
	{
		return NumConnections() == 2;
	}

	//edges
	if (m_ID < Dungeon::Instance().GetDungeonWidth() || 
		m_ID >= Dungeon::Instance().GetDungeonWidth() * Dungeon::Instance().GetDungeonHeight() - Dungeon::Instance().GetDungeonWidth() ||
		m_ID % Dungeon::Instance().GetDungeonWidth() == Dungeon::Instance().GetDungeonWidth() - 1 ||
		m_ID % Dungeon::Instance().GetDungeonWidth() == 0)
	{
		return NumConnections() == 3;
	}

	//all other cases
	return NumConnections() == 4;
}

void Room::SetRoom(Direction a_Dir, Room* a_OtherRoom)
{
	if (FullyConnected())
	{
		printf("ROOM %i IS FULLY CONNECTED", m_ID);
		return;
	}

	switch (a_Dir)
	{
	case NORTH:
		if (north == nullptr)
			north = a_OtherRoom;
		break;
	case SOUTH:
		if (south == nullptr)
			south = a_OtherRoom;
		break;
	case EAST:
		if (east == nullptr)
			east = a_OtherRoom;
		break;
	case WEST:
		if (west == nullptr)
			west = a_OtherRoom;
		break;
	default:
		//do nothing
		break;
	}
}

const int Room::GetID() const
{
	return m_ID;
}

const Direction Room::GetRandomValidDirection()
{
	int counter = 0;
	int maxCounter = 25;

	while (counter < maxCounter)
	{
		int randDir = (int)(RandNum::Instance().GetRandNum() * MAX_DIRS);

		switch (randDir)
		{
		case NORTH:
			//along top edge, can't link north
			if (m_ID < Dungeon::Instance().GetDungeonWidth())
			{
				continue;
			}

			if (north == nullptr)
			{
				return NORTH;
			}
			break;
		case SOUTH:
			//along bottom edge, can't link south
			if (m_ID >= Dungeon::Instance().GetDungeonWidth() * Dungeon::Instance().GetDungeonHeight() - Dungeon::Instance().GetDungeonWidth())
			{
				continue;
			}

			if (south == nullptr)
			{
				return SOUTH;
			}
			break;
		case EAST:
			//along right edge, can't link east
			if (m_ID % Dungeon::Instance().GetDungeonWidth() == Dungeon::Instance().GetDungeonWidth() - 1)
			{
				continue;
			}

			if (east == nullptr)
			{
				return EAST;
			}
			break;
		case WEST:
			//along left edge, can't link west
			if (m_ID % Dungeon::Instance().GetDungeonWidth() == 0)
			{
				continue;
			}

			if (west == nullptr)
			{
				return WEST;
			}
			break;
		}
		counter++;
	}
	return (Direction)-1;
}

Room* Room::GetRoom(Direction a_Direction) const
{
	Room* t = nullptr;

	switch (a_Direction)
	{
	case NORTH:
		if (north != nullptr)
		{
			t = north;
		}
		break;
	case SOUTH:
		if (south != nullptr)
		{
			t = south;
		}
		break;
	case EAST:
		if (east != nullptr)
		{
			t = east;
		}
		break;
	case WEST:
		if (west != nullptr)
		{
			t = west;
		}
		break;
	}

	return t;
}

void Room::ClearConnections()
{
	north = south = east = west = nullptr;
}

void Room::ResetStaticID()
{
	ID = 0;
}

void Room::PrintDescription() const
{
	if (!visited)
	{
		m_RoomDescription.WriteToConsole();
	}
	else
	{
		m_RoomAltDescription.WriteToConsole();
	}

	if (NumConnections() > 0)
	{
		printf("A path leads ");

		if (north != nullptr)
		{
			printf("north ");
		}

		if (south != nullptr)
		{
			printf("south ");
		}

		if (east != nullptr)
		{
			printf("east ");
		}

		if (west != nullptr)
		{
			printf("west");
		}

		printf("\n");
	}
}

void Room::ProcessItemSelection(Player* a_Player)
{
	if (m_Items.size() > 0)
	{
		PrintItems();

		m_RoomQuestion.ReadFromConsole("Will you loot from this dungeon?");

		if (m_RoomQuestion == "yes")
		{
			m_RoomQuestion.ReadFromConsole("Which one have you chosen to loot? Need exact words");

			CustomString userInput = m_RoomQuestion;

			//look at all the items
			for (auto& i : m_Items)
			{
				//look for typed input within name
				//change to lowercase chars
				CustomString loweredFullName = i.GetFullName();
				loweredFullName.ToLower();

				CustomString loweredShortName = i.GetShortName();
				loweredShortName.ToLower();

				//find returns index, otherwise returns -1
				int index1 = loweredFullName.Find(userInput);
				int index2 = loweredShortName.Find(userInput);

				if (index1 >= 0 || index2 >= 0)
				{
					m_RoomQuestion = CustomString("You have collected the ") + i.GetFullName();
					a_Player->AddItem(i.GetType());
					i.taken = true;
					break;
				}
				else
				{
					m_RoomQuestion = "That's impossible";
				}
			}
		}
		else
		{
			m_RoomQuestion = "Suit yourself...";
		}
	}
	else
	{
		m_RoomQuestion = "There are no items in this room.";
	}

	m_RoomQuestion.WriteToConsole();

	RemoveTakenItems();
}

void Room::ProcessInput(Player* a_Player)
{
	if (a_Player->UpdateInstruction())
	{
		m_RoomQuestion = "Success! Moving into next room";
	}
	else
	{
		m_RoomQuestion = "That's impossible";
	}
	m_RoomQuestion.WriteToConsole();
}

void Room::GenerateItems()
{
	int itemsToGenerate = (int)(RandNum::Instance().GetRandNum() * m_MaxItems);

	for (int i = 0; i < itemsToGenerate; ++i)
	{
		double itemChance = RandNum::Instance().GetRandNum();
		if (itemChance > 0.85)
		{
			int randType = (int)(RandNum::Instance().GetRandNum() * NUM_ITEMS);
			m_Items.push_back(Item((ItemType)randType));
		}
	}
}

void Room::PrintItems() const
{
	CustomString t = "There are items in this room. Here they are: ";
	t.WriteToConsole();

	for (unsigned int i = 0; i < m_Items.size(); ++i)
	{
		m_Items[i].Print();
	}
}

const unsigned int Room::GetNumItems() const
{
	return m_Items.size();
}

void Room::RemoveTakenItems()
{
	for (std::vector<Item>::iterator iter = m_Items.begin(); iter != m_Items.end();)
	{
		if (iter->taken)
		{
			iter = m_Items.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
