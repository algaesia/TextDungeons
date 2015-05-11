#ifndef _EMPTY_ROOM_H
#define _EMPTY_ROOMH

#include "Room.h"

class EmptyRoom : public Room
{
public:
	EmptyRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class RiddleRoom : public Room
{
public:
	RiddleRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class BossRoom : public Room
{
public:
	BossRoom();

	virtual void ProcessInput(Player* a_Player);
private:
	int chosenNum;
	CustomString strings[7];
};

class TrapRoom : public Room
{
public:
	TrapRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class TreasureRoom : public Room
{
public:
	TreasureRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class MonsterRoom : public Room
{
public:
	MonsterRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class VistaRoom : public Room
{
public:
	VistaRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class EntranceRoom : public Room
{
public:
	EntranceRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class ExitRoom : public Room
{
public:
	ExitRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class DeadEndRoom : public Room
{
public:
	DeadEndRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

class NextLevelRoom : public Room
{
public:
	NextLevelRoom();

	virtual void ProcessInput(Player* a_Player);
private:
};

#endif