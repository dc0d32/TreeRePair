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

#include "TreeRePairMacros.h"
#include "TreeRePairTypes.h"

class Digram;

/**
 * This class implements a priority queue for instances of the class
 * Digram. It is used to obtain the most frequent digram in constant
 * time on average.
 */
class DigramPriorityQueue
{
public:
	/**
	 * The constructor of the class.
	 * @param n The digram priority queue will consist of sqrt(n) subqueues.
	 *  It is supposed that n specifies the number of nodes of the input tree.
	 * @param pDigramHashMap A hash map of all diagrams occuring in the input
	 *  tree. It allows constant time access to every diagram by specifying
	 *  the parent symbol, child symbol and child index.
	 */
	DigramPriorityQueue(const unsigned int n, HASH_D_D * pDigramHashMap);
	~DigramPriorityQueue(void);

	/**
	 * This method decrements the quantity of a digram.
	 * @param pDigram the digram whose quantity shall be decremented
	 * @param subQuantity the amount by which the quantity shall be
	 *  decremented
	 */
	void decrementQuantity(Digram * pDigram, unsigned int subQuantity);

	/**
	 * This method increments the quantity of a digram.
	 * @param pDigram the digram whose quantity shall be incremented
	 * @param addQuantity the amount by which the quantity shall be
	 *  incremented
	 */
	void incrementQuantity(Digram * pDigram, unsigned int addQuantity);

	/**
	 * Sets the quantity of a digram to zero.
	 * @param pDigram the digram whose quantity shall be reset
	 */
	void setDigramQuantityToZero(Digram * pDigram);

	/**
	 * Sets the quantity of a digram to a certain value.
	 * @param pDigram the digram whose quantity shall be updated
	 * @param newQuantity the new quantity which shall be set
	 */
	void setQuantity(Digram * pDigram, const unsigned int newQuantity);

	/**
	 * This method retrieves the most frequent digram in constant
	 * time on average.
	 * @returns the most frequent digram
	 */
	Digram * top();

#ifdef DEBUG
	/**
	 * Prints the current contents of the digram priority queue.
	 * @param os the output stream to use
	 * @param pPriorityQueue the digram priority queue to print
	 */
	static void print(ostream& os, const DigramPriorityQueue * pPriorityQueue);
#endif

private:
	/**
	 * Pops a digram from the digram priority queue.
	 * @param pDigram the digram to pop
	 * @param digramListIndex the digram list of which the
	 * specified digram is part of
	 */
	void pop(Digram * pDigram, int digramListIndex);

	/**
	 * Updates the number of occurrences counter of the specified
	 * digram.
	 * @param pDigram the digram to update
	 * @param add the amout to add to the frequency counter (may
	 *  be a negative value)
	 */
	void updateNumberOfOccs(Digram * pDigram, int add);

	/**
	 * Returns the index of the digram list of which the specified
	 * digram is a member.
	 */
	int getDigramListIndex(const Digram * pDigram);

	/**
	 * Returns the score of the digram. The score is used to determine
	 * the digram list of which the digram is part of or shall be added
	 * to. At the moment, the score is just the number of occurrences
	 * of the digram. However, it can be used to fine tune the replacement
	 * step of the algorithm by adjusting the score in dependence of
	 * special properties of a digram.
	 */
	unsigned int calculateScore(const Digram * pDigram);

	/**
	 * Removes a digram from the digram hash map and deletes its instance.
	 */
	void deleteDigram(Digram * pDigram);

	/**
	 * The digram hash map allowing constant time access to the digrams.
	 */
	HASH_D_D * pDigramHashMap;

	/**
	 * The sqrt(n) digram lists.
	 */
	Digram ** pDigramLists;

	/**
	 * The number of digram lists.
	 */
	unsigned int numberOfDigramLists;

	/**
	 * Keeps track of the next non-empty digram list to check in order
	 * to retrieve the most frequent digram.
	 */
	unsigned int currentDigramListIndex;
};
