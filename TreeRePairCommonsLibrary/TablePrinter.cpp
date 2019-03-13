/*
 * TreeRePair is an implementation of the Re-pair for Trees algorithm.
 *
 * Copyright (C) 2010  Roy Mennicke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License Version 3 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not,
 * see <http://www.gnu.org/licenses/gpl-3.0.txt>.
 */
#include "TablePrinter.h"

#include "TreeRePairConstants.h"
#include "Helper.h"

#include <iostream>

void TablePrinter::init(unsigned int columnsPar, unsigned int * widthsPar)
{
	columns = columnsPar;
	widths = widthsPar;

#ifdef DEBUG
	for (unsigned int i = 0; i < columns; i++)
	{
		if (widths[i] < 3)
		{
			cout
					<< "Fatal error: Column width must be at least 3 characters. Exiting ...";
			exit(1);
		}
	}
#endif

	currentRow = "";
	currentColumn = 0;
}

void TablePrinter::addColumnData(string str)
{

	if (str.length() > widths[currentColumn])
	{
		currentRow += str.substr(0, widths[currentColumn] - 3) + "...";
	}
	else
	{
		currentRow += str;
		for (unsigned int i = 0; i < widths[currentColumn] - str.length(); i++)
		{
			currentRow += " ";
		}
	}

	for (unsigned int i = 0; i < COL_SEP; i++)
	{
		currentRow += " ";
	}

	currentColumn++;
}

void TablePrinter::addColumnData(unsigned int i)
{

	string str = Helper::stringify((size_t) i);
	addColumnData(str);

}

void TablePrinter::printRowAsHeader()
{

	printRow();

	unsigned int width = (columns - 1) * COL_SEP;
	for (unsigned int i = 0; i < columns; i++)
	{
		width += widths[i];
	}

	for (unsigned int i = 0; i < width; i++)
	{
		cout << "=";
	}

	cout << endl;

}

void TablePrinter::printRow()
{

	cout << currentRow << endl;
	currentRow = "";
	currentColumn = 0;

}
