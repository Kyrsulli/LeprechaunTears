// filehandling.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include<iostream>
#include<fstream>

int main(int argc, char** argv)
{
	std::ifstream fileName;

	
	fileName.open((char *)argv[1]);

	if(!fileName)
	{
		std::cout << "Unable to open file";
		exit(-1);
	}
	
	while(!fileName.eof())
	{
		char word[8];
		fileName >> word;
		std::cout << word << '\n';
	}
	

	int n;
	
	std::cin >> n;
	
	return 0;
}

