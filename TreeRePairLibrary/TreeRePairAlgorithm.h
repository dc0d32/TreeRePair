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
#include "DigramPriorityQueue.h"
#include "DigramCounter.h"
#include "DigramReplacer.h"
#include "Symbol.h"
#include "TreeDeleter.h"
#include "Grammar.h"
#include "TreeRePairMacros.h"

#include <iostream>

class XMLTreeParser;
class Statistics;

/*! \mainpage TreeRePair Distribution API Reference
 *
 * Please select "Classes" from the upper menu to access the documentation
 * of the methods exhibited by the classes of the TreeRePair distribution.
 */

/**
 * This class is the main class of our TreeRePair implementation.
 */
class TreeRePairAlgorithm
{
public:
	/**
	 * The constructor.
	 * @param pFilePath the input XML document for which we want
	 *  to find a small linear SLCF tree grammar representing
	 *  the corresponding XML document tree
	 */
	TreeRePairAlgorithm(char * pFilePath);

	~TreeRePairAlgorithm(void);

private:
	/**
	 * Parses the input XML document and constructs a DAG representation
	 * of the binary representation of the XML document tree in main
	 * memory.
	 */
	void parseInput();

	/**
	 * Writes the statistics of the run of the algorithm to a binary output
	 * file or/and to the standard output.
	 */
	void writeStatistics(unsigned int timeInMilliseconds,
			unsigned int numberOfInputEdges);

	/**
	 * Prunes the generated linear SLCF tree grammar (see Sect. Pruning
	 * Step of the paper).
	 */
	void pruneGrammar();

	/**
	 * Prunes the DAG representation of the input tree (i.e., all
	 * nonterminal referenced only once are eliminated).
	 */
	void pruneDagGrammar();

	/**
	 * The file containing the input XML document.
	 */
	char * pFilePath;

	/**
	 * The digram hash map allowing constant time access to the instances
	 * of the Digram instance.
	 */
	HASH_D_D * pDigramHashMap;

	/**
	 * The digram priority queue used to retrieve the most frequent digram
	 * in constant time (on average).
	 */
	DigramPriorityQueue * pPriorityQueue;

	/**
	 * Keeps track of the number of non-overlapping occurrences of each
	 * digram occuring in the input tree.
	 */
	DigramCounter * pDigramCounter;

	/**
	 * The number of nodes of the input tree.
	 */
	unsigned int nodeCount;

	/**
	 * The grammar containing the DAG representation of the input tree.
	 */
	Grammar dagGrammar;

	/**
	 * The grammar which will serve as a container for the productions
	 * generated during the run of our algorithm.
	 */
	Grammar mainGrammar;
};
