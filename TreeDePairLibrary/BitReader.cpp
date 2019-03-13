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
#include "BitReader.h"


BitReader::BitReader(ifstream * pFinPar) :
	pFin(pFinPar)
{
}

BitReader::~BitReader()
{
}

string BitReader::readBitString(unsigned int length, bool lookAhead)
{
	char buf, temp;
	while (strBuf.length() < length) {
		pFin->read(&buf, 1);
		for (short int i = 7; i >= 0; i--) {
			temp = buf;
			temp |= (0x01 << i);

			if (temp == buf) {
				strBuf += "1";
			} else {
				strBuf += "0";
			}
		}
	}

	string result = strBuf.substr(0, length);

	if (!lookAhead) {
		strBuf = strBuf.substr(length, strBuf.length() - length);
	}

	return result;
}
