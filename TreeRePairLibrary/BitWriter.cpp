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
#include "BitWriter.h"

BitWriter::BitWriter(ofstream & foutPar, bool unencodedPar) :
	fout(foutPar), buf(0x00), pos(7), unencoded(unencodedPar)
{
}

BitWriter::~BitWriter()
{
}

void BitWriter::writeBitString(const string str)
{
	if (unencoded)
	{
		fout << "[" << str << "]";
	}
	else
	{
		for (unsigned int i = 0; i < str.size(); i++)
		{
			if (pos < 0)
			{
				fout.write(&buf, 1);
				buf = 0x00;
				pos = 7;
			}

			if (str.at(i) == '1')
			{
				buf |= (0x01 << pos);
			}
			pos--;
		}
	}
}

void BitWriter::flushBuffer()
{
	fout.write(&buf, 1);
}
