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
#include "XmlTreePrinter.h"

#include "TreeRePairMode.h"
#include "TreeRePairConstants.h"
#include "TreeRePairTypes.h"
#include "Helper.h"

#include <iostream>

using namespace std;

XmlTreePrinter::XmlTreePrinter(ostream & out) :
	TreePrinter(out), useFormatting(false), convert(false)
{
}

XmlTreePrinter::~XmlTreePrinter(void)
{
}

void XmlTreePrinter::printTree(TreeNode * pRoot, bool useFormatting, bool convertPar)
{
	convert = convertPar;

	bool tempUseFormatting = this->useFormatting;
	this->useFormatting = useFormatting;

	TreePrinter::printTree(pRoot);

	this->useFormatting = tempUseFormatting;
}

void XmlTreePrinter::processNode(TreeNode * pParent, unsigned int index,
		int depth, bool closing)
{
	if (convert) {
		return processNodeConvert(pParent, index, depth, closing);
	} else {
		return processNodeDontConvert(pParent, index, depth, closing);
	}
}

void XmlTreePrinter::processNodeDontConvert(TreeNode * pParent, unsigned int index,
		int depth, bool closing)
{

	TreeNode * pNode;
	if (index != UINT_MAX)
	{
		pNode = pParent->getChild(index);
	}
	else
	{
		pNode = pParent;
	}

	std::string blank = "";
	if (useFormatting)
	{
		blank = Helper::getBlankCharacters(depth);
	}
	string endLine = Helper::getEndLine(useFormatting);

	if (pNode->getSymbol()->getRank() > 0)
	{
		if (!closing)
		{
			*out << blank << "<"
					<< Symbol::getElementTypeById(pNode->getSymbol()->getId())
					<< ">" << endLine;
		}
		else
		{
			*out << blank << "</"
					<< Symbol::getElementTypeById(pNode->getSymbol()->getId())
					<< ">" << endLine;
		}
	}
	else
	{
		if (!closing)
		{
			*out << blank << "<"
					<< Symbol::getElementTypeById(pNode->getSymbol()->getId())
					<< " />" << endLine;
		}
	}

//#ifdef DEBUG
//	out->flush();
//#endif
}

void XmlTreePrinter::processNodeConvert(TreeNode * pParent, unsigned int index,
		int depth, bool closing)
{
	std::string blank = "";
//	if (useFormatting)
//	{
//		blank = Helper::getBlankCharacters(depth);
//	}
	string endLine = Helper::getEndLine(useFormatting);

	TreeNode * pNode;
	if (index != UINT_MAX)
	{
		pNode = pParent->getChild(index);

		string name = Symbol::getElementTypeById(pNode->getSymbol()->getId());
		if (!closing) {
			if (isRightChild(pNode) && pNode->getParent()->getSymbol()->hasLeftChild()) {
				*out << blank << "</"
					<< Symbol::getElementTypeById(pNode->getParent()->getSymbol()->getId())
					<< ">" << endLine;
			}

			if (pNode->getSymbol()->hasLeftChild()) {
				*out << blank << "<" << name << ">" << endLine;
			} else {
				*out << blank << "<" << name << " />" << endLine;
			}
		} else { // closing
			if (pNode->getSymbol()->hasLeftChild() && !pNode->getSymbol()->hasRightChild()) {
				*out << blank << "</" << name << ">" << endLine;
			}

		}
	}
	else
	{
		string name = Symbol::getElementTypeById(pParent->getSymbol()->getId());
		if (!closing) {
			if (pParent->getSymbol()->hasLeftChild()) {
				*out << blank << "<" << name << ">" << endLine;
			} else {
				*out << blank << "<" << name << " />" << endLine;
			};
		} else {
			if (pParent->getSymbol()->hasLeftChild()) {
				*out << blank << "</" << name << ">" << endLine;
			}
		}
	}

//	out->flush();
}

bool XmlTreePrinter::isLeftChild(TreeNode * pNode) {
	if (pNode->getIndex() == 0 && pNode->getParent()->getSymbol()->hasLeftChild()) {
		return true;
	}
	return false;
}

bool XmlTreePrinter::isRightChild(TreeNode * pNode) {
	return !isLeftChild(pNode);
}
