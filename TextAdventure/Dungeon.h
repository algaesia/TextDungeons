#ifndef _DUNGEON_H
#define _DUNGEON_H

#include <vector>
#include "Room.h"

enum RoomType
{
	NO_ROOM = -1,
	EMPTY,
	ENTRANCE,
	EXIT,
	RIDDLE,
	BOSS,
	TRAP,
	TREASURE,
	MONSTER,
	VISTA,
	DEADEND,
	NEXTLEVEL,
};

struct PreRoom;

class Dungeon
{
public:
	static Dungeon& Instance();

	~Dungeon();

	const int GetDungeonWidth() const;
	const int GetDungeonHeight() const;

	void SetDungeonDimensions(unsigned int a_Width = 5, unsigned int a_Height = 5);

	void FillRooms();

	void PrintRooms() const;

	void ClearRooms();

	const Room* const GetStartingRoom() const;

	Room* GetRoomInDirection(unsigned int a_RoomID, Direction a_Direction) const;
	Room* GetRoom(unsigned int a_RoomID) const;
	Room* GetValidRandomRoom() const;

	bool GetDungeonFinished() const;
	void SetDungeonFinished(bool a_Finished);

	void ResetDungeon();
private:
	void SetDungeonWidth(unsigned int a_Width);
	void SetDungeonHeight(unsigned int a_Height);

	void CreatePreRooms();
	void LinkPreRooms();
	void FillPreRooms();

	Dungeon();
	Dungeon(const Dungeon&) = delete;
	void operator=(const Dungeon&) = delete;

	std::vector<Room*> m_Rooms;
	std::vector<PreRoom*> m_PreRooms;

	unsigned int m_DungeonWidth;
	unsigned int m_DungeonHeight;

	Room* m_StartingRoom;

	bool setWidth;
	bool setHeight;

	bool dungeonFinished;
};

#endif