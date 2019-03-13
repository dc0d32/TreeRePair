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
#pragma once

#include <string>

using namespace std;

/**
 * This class is used to print out a table-like representation
 * of data to the standard output.
 */
class TablePrinter
{
public:
	TablePrinter() {};
	virtual ~TablePrinter() {};

	/**
	 * Initializes the table by specifying its number of columns
	 * and their widths.
	 * @param columns the number of columns to use
	 * @param widths an array with the width for each column
	 */
	void init(unsigned int columns, unsigned int * widths);

	/**
	 * Adds string data to the current row (makes one column).
	 */
	void addColumnData(string str);

	/**
	 * Adds an integer to the current row (makes one column).
	 */
	void addColumnData(unsigned int i);

	/**
	 * Prints the current table row.
	 */
	void printRow();

	/**
	 * Prints the current row in the format of the table header.
	 */
	void printRowAsHeader();

private:
	/**
	 * The number of columns to print.
	 */
	unsigned int columns;

	/**
	 * The widths to use for the columns.
	 */
	unsigned int * widths;

	/**
	 * The current row.
	 */
	string currentRow;

	/**
	 * The column to which the data which is supplied next is added.
	 */
	unsigned int currentColumn;
};
