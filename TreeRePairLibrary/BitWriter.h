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
#include <string>

using namespace std;

/**
 * This class writes bitwise to an input stream.
 */
class BitWriter
{
public:
	/**
	 * The constructor of the class.
	 * @param fout the output stream to write to
	 * @param unencoded If true, the strings supplied are directly
	 *  written to the file (i.e., they are not regarded as bit strings).
	 *  This option is used for debugging purposes only.
	 */
	BitWriter(ofstream & fout, bool unencoded);
	virtual ~BitWriter();

	/**
	 * Writes the specified bits to the output stream.
	 */
	void writeBitString(const string str);

	/**
	 * Flushes the buffer. This method needs to be called before
	 * destruction of the BitWriter instance.
	 */
	void flushBuffer();

private:
	/**
	 * The output stream to write to.
	 */
	ofstream & fout;

	/**
	 * This 1 byte large buffer is used to write the bits to
	 * the output stream.
	 */
	char buf;

	/**
	 * The current position within the buffer.
	 */
	int pos;

	/**
	 * If true, the strings supplied are directly
	 * written to the file (i.e., they are not regarded as bit strings).
	 * This option is used for debugging purposes only.
	 */
	bool unencoded;
};
