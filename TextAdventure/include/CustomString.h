#ifndef _CUSTOM_STRING_H
#define _CUSTOM_STRING_H

class CustomString
{
public:
	CustomString(); //done //fine
	CustomString(const char* a_CharArray); //done //fine
	CustomString(const CustomString& a_OtherString); //done //fine

	~CustomString(); //done //fine

	void operator=(const CustomString& a_OtherString); //done //fine
	bool operator==(const CustomString& a_OtherString); //done //fine
	bool operator!=(const CustomString& a_OtherString);
	CustomString& operator+(const CustomString& a_OtherString); //done //fine
	CustomString& operator+=(const CustomString& a_OtherString); //done //fine

	int Length() const; //done //fine
	const char CharacterAt(int a_Index) const; //done //fine
	const char operator[](int a_Index) const;
	bool EqualTo(const CustomString& a_CustomString); //done
	const char* c_str() const; //done //fine
	void ToLower(); //done //fine
	void ToUpper(); //done //fine
	void ReadFromConsole(const char* a_StringBeforeAnswer); //done //fine
	void WriteToConsole() const; //done //fine

	bool IsEmpty() const; //done //fine
	void Append(const CustomString& a_OtherString); //done //fine
	void Prepend(const CustomString& a_OtherString); //done //fine
	int Find(const CustomString& a_OtherString);
	int Find(int a_StartIndex, const CustomString& a_OtherString);
	void Replace(const CustomString& findThis, const CustomString& replaceWith);

private:
	bool IsLetter(char a_Char); //done //fine
	bool IsNumber(char a_Char); //done //fine
	bool IsSymbol(char a_Char); //done //fine

	char* m_Chars;

	//represents total memory size of string
	int m_Capacity;
};

#endif