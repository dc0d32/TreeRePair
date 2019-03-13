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

#include "TreePrinter.h"
#include "TreeRePairMacros.h"

#include <sstream>

/**
 * Implements a XML tree printer, i.e., a given tree is transformed
 * into a XML document.
 */
class XmlTreePrinter : public TreePrinter
{
public:
	/**
	 * The constructor.
	 * @param out the output stream to use (default is cout)
	 */
	XmlTreePrinter(ostream & out = cout);
	~XmlTreePrinter(void);

	/**
	 * Initiates the printing of the tree.
	 * @param pRoot the root of the tree to print
	 * @param useFormatting specifies if every tag shall be printed on
	 *  a new line and if we shall take care of indentation
	 * @param convert if true, the tree (which is then supposed to be
	 *  a binary representation of a unranked tree) is on-the-fly
	 *  converted to the original unranked tree
	 */
	void printTree(TreeNode * pRoot, bool useFormatting, bool convert);

protected:
	void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing);

private:
	/**
	 * Used to print the tree when not converting from a binary
	 * representation.
	 */
	void processNodeDontConvert(TreeNode * pNode, unsigned int index, int depth, bool closing);

	/**
	 * Used to print the tree when converting from a binary
	 * representation of a unranked tree.
	 */
	void processNodeConvert(TreeNode * pNode, unsigned int index, int depth, bool closing);

	/**
	 * Returns true if and only if this node is a left child
	 * in the binary model..
	 */
	bool isLeftChild(TreeNode * pNode);

	/**
	 * Returns true if and only if this node is a right child
	 * in the binary model.
	 */
	bool isRightChild(TreeNode * pNode);

	/**
	 * Indicates if every tag shall be printed on
	 * a new line and if we shall take care of indentation.
	 */
	bool useFormatting;

	/**
	 * If true, the tree to print (which is then supposed to be
	 * a binary representation of a unranked tree) is on-the-fly
	 * converted to the original unranked tree (i.e., the unranked
	 * tree is written to the output file).
	 */
	bool convert;
};
