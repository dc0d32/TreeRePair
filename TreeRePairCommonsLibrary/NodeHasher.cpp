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
#include "NodeHasher.h"

#include "TreeNode.h"
    
bool NodeEqualityTester::operator()(const TreeNode* pNode1, const TreeNode* pNode2) const
{
	if (pNode1->getId() != pNode2->getId()) {
		return false;
	} else {
		return true;
	}
}

std::size_t NodeHasher::operator()(const TreeNode * pNode) const
{
	return pNode->getId();
}
