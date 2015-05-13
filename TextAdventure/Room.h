#ifndef _ROOM_H
#define _ROOM_H

#include <vector>
#include "include\CustomString.h"
#include "Player.h"
#include "Item.h"

enum Direction
{
	NO_DIRECTION = -1,
	NORTH,
	SOUTH,
	EAST,
	WEST,
	MAX_DIRS,
};

class Room
{
public:
	Room(const CustomString& a_RoomDescription = CustomString("Default"), const CustomString& a_RoomAltDescription = CustomString("Alt default"));
	Room(const Room&) = delete;

	void operator=(const Room&) = delete;

	int NumConnections() const;
	bool FullyConnected() const;
	
	void SetRoom(Direction a_Dir, Room* a_OtherRoom);
	Room* GetRoom(Direction a_Direction) const;

	const Direction GetRandomValidDirection();
	const int GetID() const;

	void ClearConnections();
	void PrintDescription() const;

	void ProcessItemSelection(Player* a_Player);

	virtual void ProcessInput(Player* a_Player);
	static void ResetStaticID();

	void RemoveTakenItems();

	void PrintItems() const;
	const unsigned int GetNumItems() const;
protected:
	bool visited;
	bool defeated;

	Room* north;
	Room* south;
	Room* east;
	Room* west;

	CustomString m_RoomDescription;
	CustomString m_RoomAltDescription;
	CustomString m_RoomQuestion;

	int m_MaxItems;
	std::vector<Item> m_Items;

	int m_ID;

	void GenerateItems();
private:
	static int ID;
};

#endif