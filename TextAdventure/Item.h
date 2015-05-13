#ifndef _ITEM_H
#define _ITEM_H

#include "include\CustomString.h"

enum ItemType
{
	NO_TYPE = -1,
	PERSEUSHELM, //helm of darkness
	TARNHELM, //helm of invisibility
	SVRIVATSA, //shield of vishnu
	PRIWEN, //shield of king arthur
	HARVESTEROFSOULS, //death's scythe
	CHENTU, //horse whip
	HEAVENSWILL, //sword of vietnamese king
	CALEDFWLCH, //similar to excalibur
	ARONDIGHT, //sword of lancelot
	MYSTELTAINN, //sword of Prainn
	GUNGNIR, //spear of odin
	SPEAROFLONGINUS,
	MJOLNIR,
	VAJRA,
	MEGINGJORD, //belt of thor
	LOTUSTREE,
	ORICHALCUM,
	NUM_ITEMS,
};

class Item
{
public:
	Item(ItemType a_Type);

	CustomString GetFullName() const;
	CustomString GetShortName() const;

	const unsigned int GetScoreValue() const;
	const ItemType GetType() const;

	void Print() const;

	bool taken;
private:
	ItemType m_Type;
	CustomString m_FullName;
	CustomString m_ShortName;
	unsigned int m_ScoreValue;
	unsigned int m_MaxScoreValue;
	unsigned int m_MinScoreValue;
};

#endif