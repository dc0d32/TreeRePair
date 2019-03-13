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
#include "SymbolInfoContainerHasher.h"

#include "SymbolInfoContainer.h"


bool SymbolInfoContainerEqualityTester::operator()(const SymbolInfoContainer* pSymbolNameContainer1,
        const SymbolInfoContainer* pSymbolNameContainer2) const
{
	if (pSymbolNameContainer1->getElementType() != pSymbolNameContainer2->getElementType())
	{
		return false;
	}

	if (pSymbolNameContainer1->hasLeftChild()
			!= pSymbolNameContainer2->hasLeftChild())
	{
		return false;
	}

	if (pSymbolNameContainer1->hasRightChild()
			!= pSymbolNameContainer2->hasRightChild())
	{
		return false;
	}

	if (pSymbolNameContainer1->getChildCount() 
		!= pSymbolNameContainer2->getChildCount()) {

		return false;
	}

	return true;
}

std::size_t SymbolInfoContainerHasher::operator()(const SymbolInfoContainer * pSymbolNameContainer) const
{
	string s = pSymbolNameContainer->getElementType();
	size_t h = 0;
	std::string::const_iterator p, p_end;
	for (p = s.begin(), p_end = s.end(); p != p_end; ++p)
	{
		h = 31 * h + (*p);
	}
	return h + pSymbolNameContainer->getChildCount();
}
