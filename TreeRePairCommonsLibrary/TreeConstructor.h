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

#include "TreeRePairTypes.h"

#include "Symbol.h"
#include "Grammar.h"
#include "XmlContentHandler.h"
#include "Production.h"
#include "TreeHasher.h"
#include "TreeDeleter.h"

#include <vector>

/**
 * This is the base class of all tree constructors.
 */
class TreeConstructor : public XmlContentHandler
{
public:
	/**
	 * The constructor of the class.
	 * @param dagGrammar the Grammar instance which serves as a
	 *  container of the generated DAG productions
	 */
	TreeConstructor(Grammar & dagGrammar);
	virtual ~TreeConstructor();

	/**
	 * Returns the number of nodes of the tree constructed.
	 */
	unsigned int getNodeCount() const {
		return nodeCount;
	};

	virtual void startElement(string & name) = 0;
	virtual void endElement(string & name) = 0;
	virtual void endDocument() = 0;

protected:
	/**
	 * Checks if a given subtree is shareable, i.e., if all subtrees
	 * rooted at the children of the specified node are already shared.
	 * @param pNode the node which shall be checked
	 * @returns true if and only if the node is shareable
	 */
	bool isShareable(TreeNode * pNode);

	/**
	 * This method checks if the given subtree is shareable and if so
	 * it checks if there was already an equal subtree encountered (in
	 * this case the subtree is replaced by a nonterminal of the DAG
	 * grammar). If it is shareable and no equal subtree was encountered
	 * until now, it is added to the subtrees hashmap in order to be
	 * able to share it at a later date.
	 * @param pNode the root of the subtree which shall be shared
	 */
	void shareTree(TreeNode * pNode);

	/**
	 * A hashmap of all shareable subtrees encountered until now.
	 */
	HASH_T_T treesHashMap;

	/**
	 * This tree deleter is used to deallocate the memory used by subtree
	 * which are shared by replacing them by a nonterminal of the DAG
	 * grammar.
	 */
	TreeDeleter treeDeleter;

	/**
	 * The DAG grammar which stores the productions of the generated
	 * DAG.
	 */
	Grammar & dagGrammar;

	/**
	 * This variable keeps track of the nodes of the tree constructed.
	 */
	int nodeCount;

	/**
	 * This variable indicates if we are constructing a binary tree
	 * (using the binary tree model) or a multiary (ranked) tree.
	 */
	bool binary;
};
