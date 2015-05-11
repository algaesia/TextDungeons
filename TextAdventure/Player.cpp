#include "Player.h"
#include "Dungeon.h"

Player::Player(const char* a_Name, int a_StartingPos)
	: m_Position(a_StartingPos),
	m_StartingPos(a_StartingPos)
{
	m_Name = CustomString(a_Name);

	//memory leak may exist here
	m_CurrentInstruction = CustomString(" ");
}

bool Player::UpdateInstruction()
{
	m_CurrentInstruction.ReadFromConsole("Which direction to go in?");
	m_CurrentInstruction.ToLower();

	return ParseInstruction();
}

bool Player::ParseInstruction()
{
	const Room* t = nullptr;

	if (m_CurrentInstruction == "north")
	{
		t = Dungeon::Instance().GetRoomInDirection(m_Position, NORTH);
	}
	else if (m_CurrentInstruction == "south")
	{
		t = Dungeon::Instance().GetRoomInDirection(m_Position, SOUTH);
	}
	else if (m_CurrentInstruction == "east")
	{
		t = Dungeon::Instance().GetRoomInDirection(m_Position, EAST);
	}
	else if (m_CurrentInstruction == "west")
	{
		t = Dungeon::Instance().GetRoomInDirection(m_Position, WEST);
	}
	else if (m_CurrentInstruction == "quit")
	{
		Dungeon::Instance().SetDungeonFinished(true);
		return true;
	}

	if (t != nullptr)
	{
		m_Position = t->GetID();
	}
	else
	{
		return false;
	}

	return true;
}

const int Player::GetCurrentPosition() const
{
	return m_Position;
}

void Player::SetCurrentPosition(int a_Position)
{
	m_Position = a_Position;
}
