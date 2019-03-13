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
#include "Symbol.h"
#include "DigramPriorityQueue.h"
#include "TreeRePairMacros.h"
#include "TreeRePairTypes.h"

#include <string>

/**
 * This class represents a digram.
 */
class Digram {
	public:
		friend class DigramPriorityQueue;

		/**
		 * This method computes the rank of the tree pattern
		 * represented by the diagram.
		 * @param pParentSymbol the parent symbol of the digram
		 * @param pChildSymbol the child symbol of the digram
		 * @return the rank computed
		 */
		static unsigned int calcNewRank(Symbol * pParentSymbol, Symbol * pChildSymbol);

		/**
		 * The constructor of the class.
		 * @param childIndex the index of the child symbol
		 * @param pParentSymbol the parent symbol of the digram
		 * @param pChildSymbol the child symbol of the digram
		 * @param pFirstOccurrence specifies the first occurrence of the digram
		 * @param pLastOccurrence specifies the last occurrence of the digram
		 */
		Digram(unsigned int childIndex, Symbol * pParentSymbol,
				Symbol * pChildSymbol, TreeNode * pFirstOccurrence,
				TreeNode * pLastOccurrence);

		~Digram(void);

		/**
		 * This method returns the parent symbol of the digram.
		 * @returns the parent symbol of the digram
		 */
		Symbol * getParentSymbol() const {
			return pParentSymbol;
		};

		/**
		 * This method returns the child symbol of the digram.
		 * @returns the child symbol of the digram
		 */
		Symbol * getChildSymbol() const {
			return pChildSymbol;
		};

		/**
		 * This method returns the index of the child symbol of the digram.
		 * @returns the index of the child symbol of the digram
		 */
		unsigned int getChildIndex() const {
			return childIndex;
		};

		/**
		 * This method gives constant time access to the first occurrence of the digram.
		 * @returns the first occurrence of the digram
		 */
		TreeNode * getFirstOccurrence() const {
			return pFirstOccurrence;
		};

		/**
		 * This method gives constant time access to the last occurrence of the digram.
		 * @returns the last occurrence of the digram
		 */
		TreeNode * getLastOccurrence() const {
			return pLastOccurrence;
		};

		/**
		 * This method updates the first occurrence of the digram.
		 * @param pFirstOccurrence the new first occurrence
		 */
		void setFirstOccurrence(TreeNode * pFirstOccurrence);

		/**
		 * This method updates the last occurrence of the digram.
		 * @param pLastOccurrence the new last occurrence
		 */
		void setLastOccurrence(TreeNode * pLastOccurrence) {
			this->pLastOccurrence = pLastOccurrence;
		};

		/**
		 * This method returns the cardinality of a maximal (with respect to
		 * cardinality) subset of non-overlapping occurrences of this digram.
		 * @returns the number of non-overlapping occurrences as it is
		 *  computed by our algorithm
		 */
		unsigned int getNumberOfOccs() const {
			return numberOfOccurrences;
		};

		/**
		 * This method returns the rank of the tree pattern
		 * represented by this diagram.
		 * @returns the rank of the tree pattern represented by this digram
		 */
		unsigned int getRankOfTreePattern() const {
			return rankOfTreePattern;
		};

		/**
		 * Prints a human readable representation of this digram.
		 * @param os the output stream to use
		 * @param pDigram the digram for which the information shall
		 *  be printed out
		 */
		static void print(ostream& os, const Digram * pDigram);

	private:
		/**
		 * Increments the number of occurrences counter of the digram.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 * @param add the amount to increment
		 */
		void incrementNumberOfOccs(unsigned int add);

		/**
		 * Decrements the number of occurrences counter of the digram.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 * @param substract the amout to decrement
		 */
		void decrementNumberOfOccs(unsigned int substract);

		/**
		 * Sets the number of occurrences counter of the digram to 0.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 */
		void setNumberOfOccsToZero() {
			numberOfOccurrences = 0;
		};

		/**
		 * Sets the number of occurrences counter of the digram.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 * @param newValue the new value to set
		 */
		void setNumberOfOccs(int newValue) {
			numberOfOccurrences = newValue;
		};

		/**
		 * Returns the preceding digram in the corresponding digram list.
		 * This method is part of the implementation of the digram lists. A
		 * digram list contains all digrams occurring equally often.
		 * Only the top digram list (see Chap. Implementation Details)
		 * contains digrams occurring not equally often.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 */
		Digram * getPrecedingDigram() const {
			return pPrecedingDigram;
		};

		/**
		 * Sets the preceding digram in the corresponding digram list.
		 * This method is part of the implementation of the digram lists. A
		 * digram list contains all digrams occurring equally often.
		 * Only the top digram list (see Chap. Implementation Details)
		 * contains digrams occurring not equally often.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 */
		void setPrecedingDigram(Digram * pDigram) {
			pPrecedingDigram = pDigram;
		};

		/**
		 * Returns the following digram in the corresponding digram list.
		 * It is part of the implementation of the digram lists. A
		 * digram list contains all digrams occurring equally often.
		 * Only the top digram list (see Chap. Implementation Details)
		 * contains digrams occurring not equally often.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 */
		Digram * getFollowingDigram() const {
#ifdef DEBUG
			if (pFollowingDigram != 0 && pFollowingDigram->getPrecedingDigram()
					!= this) {
				cout << "Fatal error when traversing digrams. Exiting ..." << endl;
				exit(1);
			}
#endif
			return pFollowingDigram;
		};

		/**
		 * Sets the following digram in the corresponding digram list.
		 * It is part of the implementation of the digram lists. A
		 * digram list contains all digrams occurring equally often.
		 * Only the top digram list (see Chap. Implementation Details)
		 * contains digrams occurring not equally often.
		 *
		 * This private method is used by the digram priority queue which is a
		 * friend class.
		 */
		void setFollowingDigram(Digram * pDigram) {
			pFollowingDigram = pDigram;
		};

		/**
		 * The parent symbol of the digram.
		 */
		Symbol * pParentSymbol;

		/**
		 * The child symbol of the digram.
		 */
		Symbol * pChildSymbol;

		/**
		 * The index of the child of the digram.
		 */
		unsigned int childIndex;

		/**
		 * The rank of the tree pattern represented by this digram.
		 */
		unsigned int rankOfTreePattern;

		/**
		 * The number of occurrences of the digram in the input tree
		 * (i.e., the cardinality of the corresponding occ_t set).
		 */
		unsigned int numberOfOccurrences;

		/**
		 * The first occurrence of the digram.
		 */
		TreeNode * pFirstOccurrence;

		/**
		 * The last occurrence of the digram.
		 */
		TreeNode * pLastOccurrence;

		/**
		 * The preceding digram in the corresponding digram list.
		 */
		Digram * pPrecedingDigram;

		/**
		 * The following digram in the corresponding digram list.
		 */
		Digram * pFollowingDigram;
};

/**
 * Compares two instances of the class Digram for equality.
 */
struct DigramEqualityTester
    : std::binary_function<Digram*, Digram*, bool>
{
    /**
     * Checks for equality of two digrams.
     * @returns returns true if the two digrams are equal
     */
    bool operator()(const Digram* pDigram1,
        const Digram* pDigram2) const
    {
    	if ((pDigram1->getParentSymbol() == 0 && pDigram1->getChildSymbol() == 0)
    			|| (pDigram2->getParentSymbol() == 0 && pDigram2->getChildSymbol() == 0)) {
    		return false;
    	}

    	if (pDigram1->getParentSymbol()->getId() != pDigram2->getParentSymbol()->getId()) {
			return false;
		}

		if (pDigram1->getChildSymbol()->getId() != pDigram2->getChildSymbol()->getId()) {
			return false;
		}

		if (pDigram1->getParentSymbol()->getRank() != pDigram2->getParentSymbol()->getRank()) {
			return false;
		}

		if (pDigram1->getChildSymbol()->getRank() != pDigram2->getChildSymbol()->getRank()) {
			return false;
		}

		if (pDigram1->getChildIndex() != pDigram2->getChildIndex()) {
			return false;
		}

		return true;
    }
};

/**
 * This class is used to compute a hash value for a given instance
 * of the class Digram.
 */
struct DigramHasher
    : std::unary_function<Digram*, std::size_t>
{
    /**
     * Computes a hash value for a given digram.
     * @param pDigram the digram for which a hash value shall be computed
     * @returns the computed hash value
     */
    std::size_t operator()(const Digram * pDigram) const
    {
    	return pDigram->getParentSymbol()->getId() + pDigram->getChildSymbol()->getId() * 128;
    }
};
