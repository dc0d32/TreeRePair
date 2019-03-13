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
#include "TreeConstructor.h"

#include <stack>
#include <string>

using namespace std;

/**
 * This class parses the input XML document and converts the contained
 * XML tree structure on-the-fly into a DAG which represents the binary
 * representation of the input tree.
 */
class BinaryTreeConstructor: public TreeConstructor
{
public:
	/**
	 * The constructor.
	 * @param grammar An instance of an linear SLCF tree grammar to which
	 *  all productions of the DAG shall be added.
	 */
	BinaryTreeConstructor(Grammar & grammar);

	~BinaryTreeConstructor(void);

	void startElement(string & name);
	void endElement(string & name);
	void endDocument();

private:
	/**
	 * This method retrieves the terminal symbol with the specified element type and
	 * children characteristic from a hash map and returns it.
	 */
	Symbol * retrieveSymbol(string elementType, bool hasLeftChild, bool hasRightChild);

	/**
	 * The root of the tree constructed.
	 */
	TreeNode * pRoot;

	/**
	 * This stack keeps track of the child index of the current XML element processed.
	 */
	stack<unsigned int*> indexStack;

	/**
	 * This stack keeps track of the parent nodes of the current node of the binary
	 * tree constructed.
	 */
	stack<TreeNode*> hierarchyStack;

	/**
	 * This stack keeps track of the element types of the XML elements for which no
	 * node in the binary tree model was created until now.
	 */
	stack<string> nameStack;
};
