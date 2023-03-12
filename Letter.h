#pragma once

#include <string>

class Letter
{
public:	
	Letter();
	Letter(char symbol);
	~Letter();

	// ����� ������ ������ ����� ��������� � ����������
	char origin;
	char technic;
	char printable;
	bool isConsonant;
	bool isVolve;
	int syll;
	int pwr;
	int number;
	int word;
	int pEnd;
	bool accent;

	std::string getLetter();
	std::string getLetterRepr();
};

