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
#include "TreeTraversal.h"

TreeTraversal::TreeTraversal(TreeNode * pRootPar) : pRoot(pRootPar)
{
}

TreeTraversal::~TreeTraversal(void)
{
}

// traverse preorder (root, then children from left to right)
void TreeTraversal::traverseTree() {
	TreeNode * pCurrentNode = pRoot;
	unsigned int index = 0;
	int depth = 0;

	processNode(pCurrentNode, UINT_MAX, depth, false);
	depth++;

	while (true) {
		if (pCurrentNode->getSymbol()->getRank() > index) {
			processNode(pCurrentNode, index, depth, false);
			pCurrentNode = pCurrentNode->getChild(index);
			depth++;
			index = 0;
		} else {
			if (pCurrentNode->getParent() != 0) {
				index = pCurrentNode->getIndex() + 1;
				TreeNode * pTemp = pCurrentNode;
				pCurrentNode = pCurrentNode->getParent();
				depth--;
				processNode(pCurrentNode, pTemp->getIndex(), depth, true);
			} else {
				break;
			}
		}
	}

	depth--;
	processNode(pCurrentNode, UINT_MAX, depth, true);
}
