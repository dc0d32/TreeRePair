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
#include "TreeDePairAlgorithm.h"

#include "Digram.h"
#include "Grammar.h"
#include "Helper.h"
#include "TreeNode.h"
#include "TreeRePairMode.h"
#include "TreeRePairConstants.h"
#include "SymbolReplacer.h"
#include "TextualGrammarReader.h"
#include "EdgeCounter.h"
#include "XmlTreePrinter.h"
#include "HuffmanSuccinctReader.h"
#include "TreeDeleter.h"

#include <fstream>
#include <time.h>
#include <cmath>
#include <string>

using namespace std;

TreeDePairAlgorithm::TreeDePairAlgorithm(char * pFilePath)
{
	// http://en.allexperts.com/q/C-1040/time-milliseconds-Windows.htm
	// the Linux g++ implementation of clock() measures CPU time used by a
	// process whereas the Visual C++ implementation measures elapsed time
	// for the process
#if !defined(__GNUC__)
	clock_t start = clock();
#else
	timespec time1, time2;
	clock_gettime(CLOCK_REALTIME, &time1);
#endif

	Grammar dagGrammar;
	Grammar mainGrammar;

	// we instantiate the start symbol so that the first
	// nonterminal of the input file becomes id 1 (instead of 0)
	Symbol::getStartSymbol();

	if (TreeRePairMode::getInstance()->useTextualGrammar()) {
		TextualGrammarReader reader(pFilePath, dagGrammar, mainGrammar);
	}
	else {
		HuffmanSuccinctReader reader(pFilePath, dagGrammar, mainGrammar);
	}

	/* Production::print(cout, dagGrammar[0]);
	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		Production::print(cout, mainGrammar[i]);
	}*/

//	unsigned int maxRank = 0;
//	Symbol * pMaxRankSymbol = 0;
//	for (unsigned int i = 0; i < Symbol::counter; i++) {
//
//		Symbol * pSymbol = (*Symbol::pSymbolByIdHashMap)[i];
//		if (maxRank < pSymbol->getRank()) {
//			maxRank = pSymbol->getRank();
//			pMaxRankSymbol = pSymbol;
//		}
//	}
//
//	cout << "Maximal rank is: " << maxRank << endl;

	SymbolReplacer symbolReplacer(dagGrammar, mainGrammar);
	SymbolReplacer::replaceNonterminals(symbolReplacer, mainGrammar);

	/*filename = Helper::getFileNamePrefix(pFilePath) + "_debug.xml";
	 ofstream fout2(filename.c_str());
	 pXmlTreePrinter->printTree(pRoot, fout2, true);
	 fout2.close();*/

	cout << "Converting binary tree to unranked tree ..." << endl;

	Production * pRootProduction = dagGrammar[0];
//	TreeConverter treeConverter(pRootProduction->getRightHandSide());
//
//	TreeNode * pOldRightHandSide = pRootProduction->getRightHandSide();
//	dagGrammar.updateRightHandSide(pRootProduction, treeConverter.getNewRoot());
//
//	TreeDeleter treeDeleter;
//	treeDeleter.deleteTree(pOldRightHandSide);

	Helper::writeUncompressedFile(pRootProduction->getRightHandSide(), (const char*)pFilePath);

	EdgeCounter edgeCounter;
	edgeCounter.countEdges(pRootProduction->getRightHandSide());
	cout << "Number of edges: " << edgeCounter.getNumberOfEdges() << endl;

#if !defined(__GNUC__)
	clock_t finish = clock();
	double clockTicks = finish - start;
	double timeInMilliseconds = clockTicks / (CLOCKS_PER_SEC / 1000);
#else
	clock_gettime(CLOCK_REALTIME, &time2);
	timespec runtime = Helper::diff(time1, time2);
	double timeInMilliseconds = runtime.tv_nsec/1000000 + runtime.tv_sec*1000;
#endif

	int timeInSeconds = (int)(timeInMilliseconds/1000);
	int minutes = (int) floor((double) timeInSeconds / 60.0);
	int seconds = timeInSeconds % 60;
	cout << "Total time needed: " << minutes << " minute(s) and " << seconds
			<< " second(s)" << endl;
}

TreeDePairAlgorithm::~TreeDePairAlgorithm(void)
{
	Helper::deleteSymbols();
	TreeRePairMode::deleteInstance();
}
