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

void Player::AddItem(ItemType a_Type)
{
	m_Items.push_back(Item(a_Type));
}

bool Player::UpdateInstruction()
{
	m_CurrentInstruction.ReadFromConsole("Which direction to go in?");

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
	else if (m_CurrentInstruction == "print items" || m_CurrentInstruction == "print")
	{
		PrintItems();
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

void Player::PrintItems() const
{
	CustomString line = "You currently have: ";
	line.WriteToConsole();

	for (const auto& i : m_Items)
	{
		i.Print();
	}
}

int Player::GetTotalScore() const
{
	int sum = 0;
	for (const auto& i : m_Items)
	{
		sum += i.GetScoreValue();
	}

	return sum;
}