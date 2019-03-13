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
#include "Grammar.h"
#include "TreeRePairTypes.h"

/**
 * The base class for all tree printers.
 */
class TreePrinter :
	public TreeTraversal
{
public:
	/**
	 * The constructor.
	 * @param out the output stream to use (default is cout)
	 * @param rawMode specifies if the printer shall work in
	 *  raw mode (implementation of this mode is printer dependend)
	 */
	TreePrinter(ostream & out = cout, bool rawMode = true);
	~TreePrinter(void);

	/**
	 * Initiate the printing of the tree.
	 * @param pNode the root of the tree to be printed
	 */
	void printTree(TreeNode * pNode);

protected:
	virtual void processNode(TreeNode * pNode, unsigned int index, int depth, bool closing) = 0;

	/**
	 * The output stream to use.
	 */
	ostream * out;

	/**
	 * Indicates if the tree printer is in raw mode.
	 */
	bool rawMode;
};
