#pragma once

#include <string>

class Letter
{
public:	
	Letter();
	Letter(std::string symbol);
	~Letter();

	// char to string
	// ����� ������ ������ ����� ��������� � ����������
	std::string origin;
	std::string technic;
	std::string printable;
	bool isConsonant;
	bool isVolve;
	int syll; // � ����� ����� ���������
	int pwr; 
	int number; // ����� �� ������� � ������
	int word; // ����� �����
	int pEnd; // ��������� ����� � ������
	bool accent;

	// w_pos ����� ����� � �����

	std::string getLetter();
	std::string getLetterRepr();

	bool operator ==(const Letter& letter) const;
};

