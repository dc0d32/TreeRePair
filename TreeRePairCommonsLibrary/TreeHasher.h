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

#include "TreeRePairMacros.h"
#include "TreeRePairTypes.h"

#include "TreeNode.h"

#include <boost/unordered_map.hpp>

/**
 * This class is used to check if two shareable subtrees (i.e., subtrees for
 * which the method TreeConstructor::isShareable returns true) are equal. In
 * order to accomplished that it tests their roots and their direct children
 * for equality.
 */
struct SubtreeEqualityTester
    : std::binary_function<TreeNode*, TreeNode*, bool>
{
    /**
     * Returns true if and only if the two shareable subtrees are equal.
     */
    bool operator()(const TreeNode* pNode1,
        const TreeNode* pNode2) const
    {
    	if (pNode1->getSymbol() != pNode2->getSymbol()) {
			return false;
		}

    	for (unsigned int i = 0; i < pNode1->getSymbol()->getRank(); i++) {
    		TreeNode * pNode1Child = pNode1->getChild(i);
    		TreeNode * pNode2Child = pNode2->getChild(i);

    		if (pNode1Child ==  0 && pNode2Child == 0) {
    			// do nothing
    		} else if (pNode1Child == 0 || pNode2Child == 0) {
    			return false;
    		} else {
				if (pNode1Child->getSymbol() != pNode2Child->getSymbol()) {
					return false;
				}
    		}
    	}

		return true;
    }
};

/**
 * This class computes a hash value for a shareable subtrees (i.e., a subtree
 * for which the method TreeConstructor::isShareable returns true).
 */
struct SubtreeHasher : std::unary_function<TreeNode*, std::size_t>
{
	/**
	 * Returns the hash value for the given shareable subtree.
	 */
    std::size_t operator()(const TreeNode * pNode) const
    {
    	std::size_t seed = 0;
		boost::hash_combine(seed, pNode->getSymbol()->getId());

		for (unsigned int i = 0; i < pNode->getCurrentChildrenCount(); i++) {
			if (pNode->getChild(i) != 0) {
				boost::hash_combine(seed, pNode->getChild(i)->getSymbol()->getId());
			}
		}

		return seed;
    }
};

