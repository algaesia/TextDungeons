#include "Dungeon.h"
#include "RandNum.h"
#include "RoomTypes.h"

struct PreRoom
{
	int roomID;
	RoomType roomType;
	int connections[4];

	PreRoom()
	{
		roomID = -1;
		roomType = NO_ROOM;
		for (int i = 0; i < 4; ++i)
			connections[i] = -1;
	}

	int NumConnections() const
	{
		int counter = 0;
		for (int i = 0; i < 4; ++i)
		{
			if (connections[i] != -1)
			{
				counter++;
			}
		}
		return counter;
	}

	bool IsFullyConnected() const
	{
		//corners
		if (roomID == 0 ||
			roomID == Dungeon::Instance().GetDungeonWidth() - 1 ||
			roomID == Dungeon::Instance().GetDungeonWidth() * Dungeon::Instance().GetDungeonHeight() - 1 ||
			roomID == Dungeon::Instance().GetDungeonWidth() * (Dungeon::Instance().GetDungeonHeight() - 1))
		{
			return NumConnections() == 2;
		}

		//edges
		if (roomID < Dungeon::Instance().GetDungeonWidth() ||
			roomID >= Dungeon::Instance().GetDungeonWidth() * Dungeon::Instance().GetDungeonHeight() - Dungeon::Instance().GetDungeonWidth() ||
			roomID % Dungeon::Instance().GetDungeonWidth() == Dungeon::Instance().GetDungeonWidth() - 1 ||
			roomID % Dungeon::Instance().GetDungeonWidth() == 0)
		{
			return NumConnections() == 3;
		}

		return NumConnections() == 4;
	}

	int GetValidRandomDirection()
	{
		int counter = 0;
		int maxCounter = 25;

		bool leaveLoop = false;

		while (counter < maxCounter)
		{
			int t = (int)(RandNum::Instance().GetRandNum() * MAX_DIRS);

			switch (t)
			{
			case NORTH:
				//along top edge, can't link north
				if (roomID < Dungeon::Instance().GetDungeonWidth())
				{
					break;
				}

				if (connections[NORTH] == -1)
				{
					return NORTH;
				}
				break;
			case SOUTH:
				//along bottom edge, can't link south
				if (roomID >= Dungeon::Instance().GetDungeonWidth() * Dungeon::Instance().GetDungeonHeight() - Dungeon::Instance().GetDungeonWidth())
				{
					break;
				}

				if (connections[SOUTH] == -1)
				{
					return SOUTH;
				}
				break;
			case EAST:
				//along right edge, can't link east
				if (roomID % Dungeon::Instance().GetDungeonWidth() == Dungeon::Instance().GetDungeonWidth() - 1)
				{
					break;
				}

				if (connections[EAST] == -1)
				{
					return EAST;
				}
				break;
			case WEST:
				//along left edge, can't link west
				if (roomID % Dungeon::Instance().GetDungeonWidth() == 0)
				{
					break;
				}

				if (connections[WEST] == -1)
				{
					return WEST;
				}
				break;
			}
			counter++;
		}

		return (Direction)-1;
	}
};

Dungeon::Dungeon()
	: 
	m_StartingRoom(nullptr),
	m_DungeonWidth(0),
	m_DungeonHeight(0),
	setWidth(false),
	setHeight(false),
	dungeonFinished(false)
{
	
}

Dungeon::~Dungeon()
{
	for (unsigned int i = 0; i < m_Rooms.size(); ++i)
	{
		//dungeon handles room destructor, deleting all rooms
		if (m_Rooms[i] != nullptr)
		{
			delete m_Rooms[i];
		}
	}

	for (unsigned int i = 0; i < m_PreRooms.size(); ++i)
	{
		delete m_PreRooms[i];
	}
}

Dungeon& Dungeon::Instance()
{
	static Dungeon instance;
	return instance;
}

void Dungeon::SetDungeonWidth(unsigned int a_Width)
{
	m_DungeonWidth = a_Width;
	setWidth = true;
}

void Dungeon::SetDungeonHeight(unsigned int a_Height)
{
	m_DungeonHeight = a_Height;
	setHeight = true;
}

bool Dungeon::GetDungeonFinished() const
{
	return dungeonFinished;
}

void Dungeon::SetDungeonFinished(bool a_Finished)
{
	dungeonFinished = a_Finished;
}

void Dungeon::CreatePreRooms()
{
	if (!setWidth && !setHeight)
	{
		printf("YOU FORGOT TO SET THE DUNGEON WIDTH AND HEIGHT\n");
		return;
	}

	int counter = 0;

	for (unsigned int i = 0; i < m_DungeonWidth; ++i)
	{
		for (unsigned int j = 0; j < m_DungeonHeight; ++j)
		{
			PreRoom* t = new PreRoom();
			t->roomID = counter;
			counter++;
			m_PreRooms.push_back(t);
		}
	}
}

void Dungeon::LinkPreRooms()
{
	if (!setWidth && !setHeight)
	{
		printf("ROOMS HAVEN'T BEEN GENERATED YET, DO THAT FIRST\n");
		return;
	}

	//all rooms are generated. now they simply need to be linked up
	//1. choose random starting point
	//2. choose random direction
	//3. keep choosing directions until:
	//			hit max width/height
	//			hit max number of iterations
	//4. go back to starting point, do step 2 again until
	//	 starting point is fully connected

	unsigned int startingX = (unsigned int)(RandNum::Instance().GetRandNum() * m_DungeonWidth);
	unsigned int startingY = (unsigned int)(RandNum::Instance().GetRandNum() * m_DungeonHeight);

	//formula for 2d -> 1d: x + height * y
	PreRoom* startRoom = m_PreRooms[startingX + m_DungeonHeight * startingY];

	startRoom->roomType = ENTRANCE;

	//current position will be used to 
	//iterate through the rooms of the 
	//dungeon as they are linked up
	PreRoom* currentRoom = nullptr;

	int firstPassCounter = 0;
	int maxFirstPassCounter = 50;

	//increases individual path length
	int secondPassCounter = 0;
	int maxSecondPassCounter = 100;

	//try link up starting room
	while (firstPassCounter < maxFirstPassCounter)
	{
		if (startRoom->IsFullyConnected())
			return;

		//start over from beginning
		currentRoom = startRoom;

		//reset second pass counter
		secondPassCounter = 0;

		//from the starting room, branch out
		while (secondPassCounter < maxSecondPassCounter)
		{
			if (currentRoom->IsFullyConnected())
				return;

			//choose random direction
			int randDir = currentRoom->GetValidRandomDirection();

			//valid direction hasn't been chosen, leave the function
			if (randDir == -1)
				return;

			PreRoom* nextRoom = nullptr;

			//link up current room in random direction
			switch (randDir)
			{
				//north
			case NORTH:
				//moving negatively on Y
				nextRoom = m_PreRooms[currentRoom->roomID - m_DungeonHeight];

				//set corresponding link backwards
				nextRoom->connections[SOUTH] = currentRoom->roomID;
				break;
				//south
			case SOUTH:
				//moving positively on Y
				nextRoom = m_PreRooms[currentRoom->roomID + m_DungeonHeight];

				//set corresponding link backwards
				nextRoom->connections[NORTH] = currentRoom->roomID;
				break;
				//east
			case EAST:
				//moving positively on X
				nextRoom = m_PreRooms[currentRoom->roomID + 1];

				//set corresponding link backwards
				nextRoom->connections[WEST] = currentRoom->roomID;
				break;
				//west
			case WEST:
				//moving negatively on X
				nextRoom = m_PreRooms[currentRoom->roomID - 1];

				//set corresponding link backwards
				nextRoom->connections[EAST] = currentRoom->roomID;
				break;
			}

			//got both current room and the next room
			//need to set currentRoom->nextRoom and nextRoom->currentRoom
			currentRoom->connections[randDir] = nextRoom->roomID;

			currentRoom = nextRoom;

			secondPassCounter++;
		}

		firstPassCounter++;
	}
}

void Dungeon::FillPreRooms()
{
	for (const auto& i : m_PreRooms)
	{
		if (i->NumConnections() > 0)
		{
			if (i->roomType == ENTRANCE)
				continue;

			if (i->NumConnections() == 1)
			{
				double n = RandNum::Instance().GetRandNum();

				if (n >= 0 && n < 0.25)
					i->roomType = DEADEND;
				else
					i->roomType = RIDDLE;
			}
			else if (i->NumConnections() == 2)
			{
				double n = RandNum::Instance().GetRandNum();

				if (n > 0.75)
					i->roomType = VISTA;
				else
					i->roomType = MONSTER;
			}
			else if (i->NumConnections() == 3)
			{
				double n = RandNum::Instance().GetRandNum();

				if (n > 0.75)
					i->roomType = TREASURE;
				else
					i->roomType = TRAP;
			}
			else if (i->NumConnections() == 4)
			{
				double n = RandNum::Instance().GetRandNum();

				if (n >= 0 && n < 0.25)
					i->roomType = TREASURE;
				else if (n >= 0.25 && n < 0.5)
					i->roomType = RIDDLE;
				else if (n > 0.9)
					i->roomType = BOSS;
				else
					i->roomType = TRAP;
			}
		}
	}

	//not yet set exit or next level
	//look through rooms that have
	//not been assigned. look at
	//the connections and link up
	//one that is valid

	int finalRoomCounter = 0;
	int maxFinalRoomTries = 5;

	while (finalRoomCounter < maxFinalRoomTries)
	{
		finalRoomCounter++;

		for (unsigned int i = 0; i < m_DungeonWidth; ++i)
		{
			for (unsigned int j = 0; j < m_DungeonHeight; ++j)
			{
				PreRoom* currentRoom = m_PreRooms[i + m_DungeonHeight * j];

				//make sure room is valid, then roll the dice
				if (currentRoom->roomType != NO_ROOM)
				{
					//chance of making exit/next level room connected here
					double roomChance = RandNum::Instance().GetRandNum();

					if (roomChance > 0.75)
					{
						double exitType = RandNum::Instance().GetRandNum();

						//choose random direction
						int randDir = currentRoom->GetValidRandomDirection();

						//valid direction hasn't been chosen, leave the function
						if (randDir == -1)
							return;

						PreRoom* nextRoom = nullptr;

						//link up current room in random direction
						switch (randDir)
						{
							//north
						case NORTH:
							//moving negatively on Y
							nextRoom = m_PreRooms[currentRoom->roomID - m_DungeonHeight];

							//set corresponding link backwards
							nextRoom->connections[SOUTH] = currentRoom->roomID;
							break;
							//south
						case SOUTH:
							//moving positively on Y
							nextRoom = m_PreRooms[currentRoom->roomID + m_DungeonHeight];

							//set corresponding link backwards
							nextRoom->connections[NORTH] = currentRoom->roomID;
							break;
							//east
						case EAST:
							//moving positively on X
							nextRoom = m_PreRooms[currentRoom->roomID + 1];

							//set corresponding link backwards
							nextRoom->connections[WEST] = currentRoom->roomID;
							break;
							//west
						case WEST:
							//moving negatively on X
							nextRoom = m_PreRooms[currentRoom->roomID - 1];

							//set corresponding link backwards
							nextRoom->connections[EAST] = currentRoom->roomID;
							break;
						}

						//got both current room and the next room
						//need to set currentRoom->nextRoom and nextRoom->currentRoom
						currentRoom->connections[randDir] = nextRoom->roomID;

						if (exitType > 0.5)
						{
							nextRoom->roomType = EXIT;
						}
						else
						{
							nextRoom->roomType = NEXTLEVEL;
						}

						//setting up exit is done, leave function
						return;
					}
				}
			}
		}
	}
}

const int Dungeon::GetDungeonWidth() const
{
	return m_DungeonWidth;
}

const int Dungeon::GetDungeonHeight() const
{
	return m_DungeonHeight;
}

void Dungeon::PrintRooms() const
{
	for (unsigned int i = 0; i < m_Rooms.size(); ++i)
	{
		printf("Room %i connected to: ", m_Rooms[i]->GetID());

		if (m_Rooms[i]->GetRoom(NORTH) != nullptr)
			printf("%i ", m_Rooms[i]->GetRoom(NORTH)->GetID());

		if (m_Rooms[i]->GetRoom(SOUTH) != nullptr)
			printf("%i ", m_Rooms[i]->GetRoom(SOUTH)->GetID());

		if (m_Rooms[i]->GetRoom(EAST) != nullptr)
			printf("%i ", m_Rooms[i]->GetRoom(EAST)->GetID());

		if (m_Rooms[i]->GetRoom(WEST) != nullptr)
			printf("%i ", m_Rooms[i]->GetRoom(WEST)->GetID());

		printf("\n");
	}
}

const Room* const Dungeon::GetStartingRoom() const
{
	if (!setHeight && !setWidth)
		printf("DUNGEON HASN'T BEEN INITIALISED CORRECTLY YET\n");

	if (m_StartingRoom == nullptr)
		printf("STARTING ROOM HASN'T BEEN ASSIGNED YET\n");

	return m_StartingRoom;
}

void Dungeon::FillRooms()
{
	CreatePreRooms();
	LinkPreRooms();
	FillPreRooms();

	//look at pre rooms id and create new rooms
	//based on the type of pre room
	//need to create the rooms first, then link them
	for (const auto& i : m_PreRooms)
	{
		switch (i->roomType)
		{
		case NO_ROOM:
			m_Rooms.push_back(new Room());
			break;
		case EMPTY:
			m_Rooms.push_back(new EmptyRoom());
			break;
		case RIDDLE:
			m_Rooms.push_back(new RiddleRoom());
			break;
		case BOSS:
			m_Rooms.push_back(new BossRoom());
			break;
		case TRAP:
			m_Rooms.push_back(new TrapRoom());
			break;
		case TREASURE:
			m_Rooms.push_back(new TreasureRoom());
			break;
		case MONSTER:
			m_Rooms.push_back(new MonsterRoom());
			break;
		case VISTA:
			m_Rooms.push_back(new VistaRoom());
			break;
		case ENTRANCE:
			m_StartingRoom = new EntranceRoom();
			m_Rooms.push_back(m_StartingRoom);
			break;
		case EXIT:
			m_Rooms.push_back(new ExitRoom());
			break;
		case DEADEND:
			m_Rooms.push_back(new DeadEndRoom());
			break;
		case NEXTLEVEL:
			m_Rooms.push_back(new NextLevelRoom());
			break;
		}
	}

	//all rooms are created, now to link them
	for (unsigned int i = 0; i < m_Rooms.size(); ++i)
	{
		int roomNum = -1;

		if (m_PreRooms[i]->connections[NORTH] != -1)
		{
			roomNum = m_PreRooms[i]->connections[NORTH];
			m_Rooms[i]->SetRoom(NORTH, m_Rooms[roomNum]);
		}

		if (m_PreRooms[i]->connections[SOUTH] != -1)
		{
			roomNum = m_PreRooms[i]->connections[SOUTH];
			m_Rooms[i]->SetRoom(SOUTH, m_Rooms[roomNum]);
		}

		if (m_PreRooms[i]->connections[EAST] != -1)
		{
			roomNum = m_PreRooms[i]->connections[EAST];
			m_Rooms[i]->SetRoom(EAST, m_Rooms[roomNum]);
		}

		if (m_PreRooms[i]->connections[WEST] != -1)
		{
			roomNum = m_PreRooms[i]->connections[WEST];
			m_Rooms[i]->SetRoom(WEST, m_Rooms[roomNum]);
		}
	}
}

void Dungeon::ClearRooms()
{
	for (Room* i : m_Rooms)
	{
		i->ClearConnections();
	}
}

Room* Dungeon::GetRoomInDirection(unsigned int a_RoomID, Direction a_Direction) const
{
	return m_Rooms[a_RoomID]->GetRoom(a_Direction);
}

Room* Dungeon::GetRoom(unsigned int a_RoomID) const
{
	if (a_RoomID >= 0 && a_RoomID < m_Rooms.size())
		return m_Rooms[a_RoomID];

	return nullptr;
}

void Dungeon::ResetDungeon()
{
	Room::ResetStaticID();

	for (unsigned int i = 0; i < m_PreRooms.size(); ++i)
	{
		delete m_PreRooms[i];
		m_PreRooms[i] = nullptr;
	}

	m_PreRooms.clear();

	for (unsigned int i = 0; i < m_Rooms.size(); ++i)
	{
		delete m_Rooms[i];
		m_Rooms[i] = nullptr;
	}

	m_Rooms.clear();

	FillRooms();
}

Room* Dungeon::GetValidRandomRoom() const
{
	int counter = 0;
	int maxCounter = 5;

	while (counter < maxCounter)
	{
		unsigned int roomX = (unsigned int)(RandNum::Instance().GetRandNum() * m_DungeonWidth);
		unsigned int roomY = (unsigned int)(RandNum::Instance().GetRandNum() * m_DungeonHeight);

		if (m_PreRooms[roomX + m_DungeonHeight * roomY]->roomType != -1)
		{
			return m_Rooms[roomX + m_DungeonHeight * roomY];
		}

		counter++;
	}

	return nullptr;
}

void Dungeon::SetDungeonDimensions(unsigned int a_Width, unsigned int a_Height)
{
	SetDungeonWidth(a_Width);
	SetDungeonHeight(a_Height);
}
