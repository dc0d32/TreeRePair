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

#include <fstream>

using namespace std;

/**
 * This class reads bitwise from an input stream.
 */
class BitReader
{
public:

	/**
	 * The constructor of the class.
	 * @param pFinPar the input stream to read from
	 */
	BitReader(ifstream * pFinPar);

	virtual ~BitReader();

	/**
	 * This methods returns a certain number of bits from the input stream
	 * and optionally reads (i.e., deletes) them from the input stream.
	 * @param length The number of bits to read. If the end of the input
	 *  stream is reached, a smaller number of bits is returned.
	 * @param lookAhead This optional parameter specifies if the bits
	 * 	read from the input stream shall be absorbed. The default is to
	 *  not absorbed them, i.e., the next call to this method will
	 *  return them again.
	 * @returns the bits read are returned as a string
	 */
	string readBitString(unsigned int length, bool lookAhead = false);

private:
	/**
	 * The input stream which is read bitwise.
	 */
	ifstream * pFin;

	/**
	 * The buffer which is used to read the input stream.
	 */
	string strBuf;
};
