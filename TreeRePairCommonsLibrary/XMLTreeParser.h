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
#include "TreeConstructor.h"
#include "TreeRePairMacros.h"
#include "TreeRePairTypes.h"
#include "Grammar.h"

/**
 * This class constructs a DAG representation of a ranked tree which is
 * retrieved from the given XML document (tree).
 */
class XMLTreeParser
{
public:
	/**
	 * The constructor of the class.
	 * @param pFilePath the XML document from which the tree shall be retrieved
	 * @param dagGrammar the grammar which will later contain all DAG productions
	 * @param binary indicates if we create a DAG from the binary representation
	 *  of the XMl document tree or from the multiary (ranked) representation of
	 *  it (default is true)
	 */
	XMLTreeParser(char * pFilePath, Grammar & dagGrammar, bool binary = true);
	~XMLTreeParser(void);

	/**
	 * Returns the number of nodes of the XML document tree.
	 */
	unsigned int getNodeCount() const { return nodeCount; };
private:
	/**
	 * The grammar which serves as a container for all DAG productions.
	 */
	Grammar & dagGrammar;

	/**
	 * Keeps track of the number of nodes of the input tree.
	 */
	unsigned int nodeCount;
};
