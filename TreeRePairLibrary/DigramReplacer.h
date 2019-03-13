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

#include "DigramPriorityQueue.h"
#include "TreeRePairMacros.h"
#include "Grammar.h"
#include "DigramCounter.h"
#include "Digram.h"
#include "TreeNode.h"
#include "Symbol.h"
#include "Helper.h"
#include "OccurrenceFinder.h"
#include "Grammar.h"
#include "Production.h"

/**
 * Implements the replacement step of the Re-pair for Trees
 * algorithm.
 */
class DigramReplacer {
public:
	/**
	 * the constructor of the class.
	 * @param pPriorityQueue the digram priority queue which is used
	 *  to retrieve the most frequent digram in constant time (on average)
	 * @param pDigramCounter the digram counter which keeps track of the
	 *  set of non-overlapping occurrences for each digram
	 * @param pDigramHashMap the digram hash map allowing constant time
	 *  access to the Digram instances
	 * @param dagGrammar the grammar containing the input tree
	 * @param mainGrammar the grammar which will serve as a container for the
	 *  productions generated during the digram replacement step
	 */
	DigramReplacer(DigramPriorityQueue * pPriorityQueue,
			DigramCounter * pDigramCounter, HASH_D_D * pDigramHashMap,
			Grammar & dagGrammar,
			Grammar & mainGrammar);
	~DigramReplacer(void);

	/**
	 * Creates a DAG production with the specified right-hand side.
	 * @param dagGrammar the grammar to which the generated production shall
	 *  be added
	 * @param pNode the root of the tree which will become the right-hand
	 *  side
	 * @param transitiveRefCount the transitive reference count to set
	 * @returns the generated production
	 */
	static Production * createDagProduction(Grammar & dagGrammar,
			TreeNode * pNode, unsigned int transitiveRefCount);

private:
	/**
	 * Initiates the replacement step.
	 */
	void replaceDigrams();

	/**
	 * Replaces the occurrences of the specified digram.
	 * @param pDigram the digram to be replaced
	 * @param pNonterminal the nonterminal which is used to replace the occurrences
	 *  of the digram
	 * @param counter indicates that this is the digramCounter th digram
	 *  which is replaced during the replacement step (for debugging purposes only)
	 * @returns the production which represents the tree pattern replaced
	 */
	Production * replaceDigramOccurrences(Digram * pDigram, Symbol * pNonterminal,
			int counter);

	/**
	 * Replaces an occurrence of a digram by a node labeled by the specified
	 * nonterminal.
	 * @param pOccurrence the occurrence to be replaced
	 * @param pDigram the digram whose occurrence is to be replaced
	 * @param pNonterminal the nonterminal which is used as a label for
	 *  the node to be introduced
	 * @returns the new node labeled by the nonterminal
	 */
	TreeNode * replaceOccurrencesAux(TreeNode * pOccurrence,
				Digram * pDigram, Symbol * pNonterminal);

	/**
	 * Removes all absorbed occurrences from the sets of non-overlapping
	 * occurrences of all digrams involved in the occurrence replacement.
	 * @param pDigram the digram whose occurrence is to be replaced
	 * @param pOccurrence the occurrence to be replaced
	 */
	void deregisterAbsorbedOccurrences(Digram * pDigram, TreeNode * pOccurrence);

	/**
	 * Updates the input tree (or what is remaining of it) by introducing
	 * a new node labeled by the specified nonterminal and assigning it
	 * all children of the two nodes replaced (i.e., of the occurrence
	 * to be replaced).
	 */
	TreeNode * updateTree(Symbol * pNonterminal, Digram * pDigram,
				TreeNode * pOccurrence);

	/**
	 * Adds all new occurrences which emerged due to the occurrence
	 * replacement to the sets of non-overlapping occurrences of all
	 * digrams involved in the occurrence replacement.
	 * @param pDigram the digram whose occurrence is to be replaced
	 * @param pNode the new node which replaced the digram occurrence
	 *  and which is labeled by the new nonterminal
	 */
	void registerNewOccurrences(Digram * pDigram, TreeNode * pNode);

	/**
	 * Creates a new nonterminal symbol with the rank of the tree pattern
	 * represented by the specified pDigram.
	 */
	Symbol * createNewNonterminal(Digram * pDigram);

#ifdef DEBUG
	void checkProductions();
	void checkOccurrences(HASH_D_D * pDigramHashMap, bool showDebugInfo = false);
	void checkOccurrencesOfDigram(Digram * pDigram, bool showDebugInfo);
	void checkOccurrenceWithDigram(TreeNode * pOccurrence, Digram * pDigram,
			int counter);
	unsigned int countOccurrences(Digram * pDigram);
#endif

	/**
	 * The digram priority queue which is used to retrieve the most
	 * frequent digram at every iteration of the replacement step.
	 */
	DigramPriorityQueue * pPriorityQueue;

	/**
	 * The digram counter which is used to update the digram occurrence
	 * queues.
	 */
	DigramCounter * pDigramCounter;

	/**
	 * The digram hash map which allows constant time access to the instances
	 * of the Digram class using their parent and child label and their index.
	 */
	HASH_D_D * pDigramHashMap;

	/**
	 * The grammar containing a DAG representation of the input tree.
	 */
	Grammar & dagGrammar;

	/**
	 * The grammar which serves as a container for the productions created
	 * during the replacement step.
	 */
	Grammar & mainGrammar;

	/**
	 * Keeps track of how many digrams where replaced so far.
	 */
	int counter;
};
