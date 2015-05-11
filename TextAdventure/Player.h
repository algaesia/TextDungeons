#ifndef _PLAYER_H
#define _PLAYER_H

#include "include\CustomString.h"

class Player
{
public:
	Player(const char* a_Name, int a_StartingPos);

	bool UpdateInstruction();

	const int GetCurrentPosition() const;
	void SetCurrentPosition(int a_Position);
private:
	bool ParseInstruction();

	CustomString m_Name;

	CustomString m_CurrentInstruction;

	//represented with room ID
	int m_Position;

	int m_StartingPos;
};

#endif