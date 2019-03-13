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

#include "TreeTraversal.h"
#include "TreeNode.h"
#include "DigramPriorityQueue.h"
#include "Grammar.h"

/**
 * This class helps to keep track of the occurrences of the digrams.
 * It implements an algorithm to obtain a maximal (with respect to
 * cardinality) non-overlapping set of occurrences for a given
 * digram. It allows occurrences to be added and removed from the
 * sets occ_t.
 */
class DigramCounter: public TreeTraversal {
public:
	/**
	 * The constructor of the class. It initiates the initial construction
	 * of the sets occ_t (where t is the input tree).
	 * @param dagGrammar the grammar containing the input tree (represented
	 *  by a DAG)
	 * @param pDigramHashMap a hash map allowing constant access to the
	 *  Digram instances by specifying the parent symbol, child symbol and
	 *  index of the digram
	 * @param pPriorityQueue the priority queue used to retrieve the most
	 *  frequent digram
	 */
	DigramCounter(Grammar & dagGrammar,	HASH_D_D * pDigramHashMap,
			DigramPriorityQueue * pPriorityQueue);
	~DigramCounter(void);

	/**
	 * Adds the specified node to the set occ_t sets of all digrams which
	 * are formed by the label of the specified node and the labels of its
	 * children (provided that the specified node is no overlapping
	 * occurrence).
	 */
	void registerOccurrences(TreeNode * pNode);

	/**
	 * Adds the specified node to the set occ_t of the digram formed by
	 * the label of the specified node and its child with the specified
	 * index (as long as the specified node is no overlapping occurrence).
	 */
	void registerOccurrence(TreeNode * pNode, unsigned int index);

	/**
	 * Removes the specified node from the set occ_t of the digram formed
	 * by the label of the specified node and its child with the specified
	 * index (as long as this digram is not equal to the specified digram).
	 * @param pNode the occurrence to remove
	 * @param index the index of the digram whose occurrences set shall be
	 *  updated
	 * @param pSkipDigram if the digram formed by the label of the specified node
	 *  the its child with the specified index is equal to this digram, then
	 *  the occurrence is not removed from the set of occurrences
	 */
	void deregisterOccurrence(TreeNode * pNode, unsigned int index, Digram * pSkipDigram);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth,
				bool closing);

private:
	/**
	 * Updates the occurrences list of the specified digram
	 * to reflect the disappearance of the specified occurrence.
	 * @param pDigram the digram to which the specified occurrence belongs
	 * @param pOccurrence the occurrence which will disappear
	 */
	void updateOccurrencesList(Digram * pDigram, TreeNode * pOccurrence);

	/**
	 * Returns true if and only if the specified occurrence is
	 * non-overlapping.
	 */
	bool isNonOverlapping(TreeNode * pNode, unsigned int index);

	/**
	 * The digram hash map allowing constant time access to the Digram instances.
	 */
	HASH_D_D * pDigramHashMap;

	/**
	 * The digram priority queue. It keeps track of the number of occurrences
	 * of each digram.
	 */
	DigramPriorityQueue * pPriorityQueue;

	/**
	 * The grammar containing the input tree (or what is remaining of it).
	 */
	Grammar & dagGrammar;

	/**
	 * The production whose right-hand side is to be traversed by the digram
	 * counter in order to register the occurrences of the occurring digrams.
	 */
	Production * pProduction;
};
