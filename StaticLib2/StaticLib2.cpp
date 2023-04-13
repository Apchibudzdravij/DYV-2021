#include "pch.h"
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <Windows.h>
extern "C"
{

	int __stdcall power(int a, int b)
	{
		return pow(a, b);
	}
	int __stdcall strcomp(char* a, char* b, int ai, int bi)
	{
		for (int i = 0; (i < ai) && (i < bi); ++i)
		{
			if (a[i] != b[i])
				return -1;
		}
		return 1;
	}
	void __stdcall pausese()
	{
		system("pause");
		return;
	}
	char* _stdcall _ConvertToChar(int number)
	{
		if (number == 0)
			return (char*)"0";
		char* temp = (char*)calloc(255, sizeof(char));
		char* buffer = (char*)calloc(255, sizeof(char));
		if (temp == NULL)
			return NULL;
		if (buffer == NULL)
			return NULL;

		int bin = number, i = 0;
		if (number < 0)
		{
			unsigned long cumber = number * (-1);
			bin = cumber;
		}
		while (bin)
		{
			temp[i++] = (bin % 10) + '0'; 
			bin /= 10;
		}

		if (number < 0)
		{
			buffer[0] = '-';
			for (size_t i = 1, j = strlen(temp) - 1; i <= strlen(temp); i++, j--)
				buffer[i] = temp[j];
		}
		else
		{
			for (size_t i = 0, j = strlen(temp) - 1; i < strlen(temp); i++, j--)
				buffer[i] = temp[j];
		}

		buffer[++i] = '\0';
		return buffer;
	}
	char* __stdcall IntToHexChar(int number)
	{
		int rem;
		int len = 0;
		int bufff = number;
		while (bufff != 0)
		{
			++len;
			bufff = bufff / 10;
		}
		int i = 2;
		char* hexaDecNum = new char[len];
		hexaDecNum[0] = '0';
		hexaDecNum[1] = '1';
		while (number != 0)
		{
			rem = number % 16;
			if (rem < 10)
				rem = rem + 48;
			else
				rem = rem + 55;
			if (len > 2)
				hexaDecNum[len + 1 - i] = rem;
			else
				hexaDecNum[len + 2 - i] = rem;
			i++;
			number = number / 16;
		}
		if (i - 2 > 2)
			hexaDecNum[i] = '\0';
		else
			hexaDecNum[i + 1] = '\0';
		return hexaDecNum;
	}

	int __stdcall outnum(int value)
	{
		std::cout << value;
		return 0;
	}
	int __stdcall outstr(char* ptr)
	{
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		return 0;
	}
	int __stdcall outnumline(int value)
	{
		std::cout << value << std::endl;
		return 0;
	}
	int __stdcall outstrline(char* ptr)
	{
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		std::cout << std::endl;
		return 0;
	}
	int __stdcall system_pause()
	{
		system("pause");
		return 0;
	}
}