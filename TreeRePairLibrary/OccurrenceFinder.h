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

#include "TreeNode.h"
#include "Digram.h"

/**
 * This class is used to retrieve the next occurrence of a digram.
 */
class OccurrenceFinder {
public:
	/**
	 * The constructor.
	 * @param pDigram The digram for which the occurrences shall be retrieved.
	 */
	OccurrenceFinder(Digram * pDigram);
	~OccurrenceFinder();

	/**
	 * Returns the next occurrence relative to the specified occurrence.
	 * @param pOccurrence if null, the first occurrence of the digram is
	 *  returned
	 * @returns the next occurrence; if their is no next occurrence, a null
	 *  pointer is returned
	 */
	TreeNode * getNextOccurrence(TreeNode * pOccurrence);

private:
	/**
	 * The digram for which the occurrences shall be retrieved.
	 */
	Digram * pDigram;
};
