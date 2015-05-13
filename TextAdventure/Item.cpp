#include "Item.h"
#include "RandNum.h"
#include <string>

Item::Item(ItemType a_Type)
	: m_FullName("Default"),
	m_ShortName("Default"),
	m_ScoreValue(0),
	m_Type(a_Type),
	m_MinScoreValue(0),
	m_MaxScoreValue(100),
	taken(false)
{
	switch (m_Type)
	{
	case NO_TYPE:
		m_FullName = "No item";
		m_ShortName = "No item";
		m_ScoreValue = 0;
		break;
	case PERSEUSHELM: //helm of darkness
		m_FullName = "Helm of Perseus";
		m_ShortName = "Perseus";
		m_MinScoreValue = 100;
		m_MaxScoreValue = 500;
		break;
	case TARNHELM: //helm of invisibility
		m_FullName = "Tarn Helm";
		m_ShortName = "Tarn";
		m_MinScoreValue = 250;
		m_MaxScoreValue = 700;
		break;
	case SVRIVATSA: //shield of vishnu
		m_FullName = "Svrivatsa, shield of Vishnu";
		m_ShortName = "Svrivatsa";
		m_MinScoreValue = 200;
		m_MaxScoreValue = 350;
		break;
	case PRIWEN: //shield of king arthur
		m_FullName = "Priwen, King Arthur's defender";
		m_ShortName = "Priwen";
		m_MinScoreValue = 350;
		m_MaxScoreValue = 500;
		break;
	case HARVESTEROFSOULS: //death's scythe
		m_FullName = "Harvester of Souls";
		m_ShortName = "Harvester";
		m_MinScoreValue = 450;
		m_MaxScoreValue = 650;
		break;
	case CHENTU: //horse whip
		m_FullName = "Chentu the horse whip";
		m_ShortName = "Chentu";
		m_MinScoreValue = 300;
		m_MaxScoreValue = 800;
		break;
	case HEAVENSWILL: //sword of vietnamese king
		m_FullName = "Heaven's Will, sword of Le Loi";
		m_ShortName = "Heaven's Will";
		m_MinScoreValue = 370;
		m_MaxScoreValue = 550;
		break;
	case CALEDFWLCH: //similar to excalibur
		m_FullName = "Caledfwlch, Scotland's Excalibur?";
		m_ShortName = "Caledfwlch";
		m_MinScoreValue = 560;
		m_MaxScoreValue = 710;
		break;
	case ARONDIGHT: //sword of lancelot
		m_FullName = "Arondight, sword of Lancelot";
		m_ShortName = "Arondight";
		m_MinScoreValue = 480;
		m_MaxScoreValue = 830;
		break;
	case MYSTELTAINN: //sword of Prainn
		m_FullName = "Mysteltainn, Prainn's destroyer";
		m_ShortName = "Mysteltainn";
		m_MinScoreValue = 600;
		m_MaxScoreValue = 990;
		break;
	case GUNGNIR: //spear of odin
		m_FullName = "Gungnir, spear of Odin";
		m_ShortName = "Gungnir";
		m_MinScoreValue = 450;
		m_MaxScoreValue = 900;
		break;
	case SPEAROFLONGINUS:
		m_FullName = "Spear of Longinus, piercer of Christ";
		m_ShortName = "Longinus";
		m_MinScoreValue = 500;
		m_MaxScoreValue = 850;
		break;
	case MJOLNIR:
		m_FullName = "Mjolnir, hammer of the gods";
		m_ShortName = "Mjolnir";
		m_MinScoreValue = 450;
		m_MaxScoreValue = 770;
		break;
	case VAJRA:
		m_FullName = "Vajra";
		m_ShortName = "Vajra";
		m_MinScoreValue = 400;
		m_MaxScoreValue = 600;
		break;
	case MEGINGJORD: //belt of thor
		m_FullName = "Megingjord, belt of Thor";
		m_ShortName = "Megingjord";
		m_MinScoreValue = 400;
		m_MaxScoreValue = 900;
		break;
	case LOTUSTREE:
		m_FullName = "Leaf of the Lotus Tree";
		m_ShortName = "Leaf";
		m_MinScoreValue = 250;
		m_MaxScoreValue = 450;
		break;
	case ORICHALCUM:
		m_FullName = "Orichalcum";
		m_ShortName = "Orichalcum";
		m_MinScoreValue = 450;
		m_MaxScoreValue = 690;
		break;
	}

	m_ScoreValue = m_MinScoreValue + (unsigned int)(RandNum::Instance().GetRandNum() * (m_MaxScoreValue - m_MinScoreValue));
}

CustomString Item::GetFullName() const
{
	return m_FullName;
}

CustomString Item::GetShortName() const
{
	return m_ShortName;
}

const unsigned int Item::GetScoreValue() const
{
	return m_ScoreValue;
}

const ItemType Item::GetType() const
{
	return m_Type;
}

void Item::Print() const
{
	CustomString temp = CustomString("Name: ") + m_FullName + ". Value: " + std::to_string(m_ScoreValue).c_str();
	temp.WriteToConsole();
}
