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

#include "TreeConstructor.h"

#include <stack>

/**
 * This class parses the input XML document and converts the contained
 * XML tree structure on-the-fly into a DAG of a ranked representation
 * of the input tree (i.e., for every unique element type - number of
 * children combination a new symbol is introduced).
 */
class MultiaryTreeConstructor : public TreeConstructor
{
public:
	/**
	 * The constructor.
	 * @param grammar An instance of an linear SLCF tree grammar to which
	 *  all productions of the DAG shall be added.
	 */
	MultiaryTreeConstructor(Grammar & grammar);
	~MultiaryTreeConstructor(void);

	void startElement(string & name);
	void endElement(string & name);
	void endDocument();

private:
	/**
	 * The stack keeping track of the children of nodes which are still
	 * to be constructed.
	 */
	stack<vector<TreeNode*>*> childrenStack;
};
