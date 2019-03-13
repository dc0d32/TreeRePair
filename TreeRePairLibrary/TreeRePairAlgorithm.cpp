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
#include "TreeRePairAlgorithm.h"

#include "XMLTreeParser.h"

#include "Helper.h"
#include "Statistics.h"

#include "Digram.h"
#include "Grammar.h"
#include "GrammarPruner.h"
#include "ReferencesTreeCounter.h"
#include "PlaceholderInserter.h"
#include "TreeGenerator.h"

#include "EdgeCounter.h"

#include "TextualGrammarWriter.h"
#include "BplexGrammarWriter.h"
#include "HuffmanSuccinctWriter.h"

#include "TreeRePairMode.h"
#include "TreeRePairConstants.h"

#include <fstream>
#include <time.h>
#include <string>

using namespace std;

TreeRePairAlgorithm::TreeRePairAlgorithm(char * pFilePathPar) :
	pFilePath(pFilePathPar), nodeCount(0)
{
	TreeRePairMode::getInstance()->setAlgMode(TreeRePairMode::COMPRESS);

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

	if (TreeRePairMode::getInstance()->isVerboseMode())
	{
		Helper::printCurrentMode();
	}

	pDigramHashMap = new HASH_D_D;

	parseInput();
	
	// for testing purposes only
	// DegeneratedTreeGenerator generator(dagGrammar, 8192);
	// BinaryTreeGenerator2 generator(dagGrammar);
	// EdgeCounter counter;
	// counter.countEdges(dagGrammar);
	// nodeCount = counter.getNumberOfEdges() + 1;

	// Production::print(cout, dagGrammar[0]);

	/*for (unsigned int i = 0; i < dagGrammar.size(); i++) {
		Production * pProduction = dagGrammar[i];
		cout << "ref count: " << pProduction->getRefCount() << endl;
		Production::print(cout, pProduction);
	}*/

	int numberOfInputEdges = nodeCount - 1;
	if (TreeRePairMode::getInstance()->isVerboseMode())
	{
		cout << "XML document tree contains " << numberOfInputEdges << " edges."
				<< endl;
	}

	if (TreeRePairMode::getInstance()->isUseDag())
	{
		pruneDagGrammar();
	}

//	dag unfolding test
//	vector<Production*> * pProductions = pDagGrammar->getProductions();
//	TreeCopier treeCopier((*pProductions)[0]->getRightHandSide());
//	TreeNode * pNewRoot = copier.getNewRoot();
//	TreeConverter treeConverter(pNewRoot);
//	pNewRoot = treeConverter.getNewRoot();
//
//	string filename = Helper::getFilePathForUncompressed("unfold.xml");
//
//	XmlTreePrinter xmlTreePrinter(cout);
//	ofstream fout(filename.c_str());
//	xmlTreePrinter.printTree(pNewRoot, fout);
//	fout.close();

	// experimental
	// PlaceholderInserter inserter(dagGrammar);

	pPriorityQueue = new DigramPriorityQueue(nodeCount, pDigramHashMap);

	if (TreeRePairMode::getInstance()->isVerboseMode())
	{
		cout << "Counting digram occurrences initially ... " << endl;
	}

	pDigramCounter = new DigramCounter(dagGrammar, pDigramHashMap, pPriorityQueue);

	if (TreeRePairMode::getInstance()->isVerboseMode())
	{
		cout << "Replacing digrams ..." << endl;
	}

	DigramReplacer digramReplacer(pPriorityQueue, pDigramCounter,
			pDigramHashMap, dagGrammar, mainGrammar);

	// Helper::printDigramsHashMap(pDigramHashMap);
	// Production::print(cout, dagGrammar[0]);


	if (TreeRePairMode::getInstance()->getOptimizationMode() != TreeRePairMode::OPTIMIZE_DISABLE) {
		pruneGrammar();
	}

	//for (unsigned int i = 0; i < dagGrammar.size(); i++) {
	//	Production * pProduction = dagGrammar[i];
	//	cout << "ref count: " << pProduction->getRefCount() << endl;
	//	Production::print(cout, pProduction);
	//}

	HuffmanSuccinctWriter succinctWriter(dagGrammar, mainGrammar, pFilePath);

	if (TreeRePairMode::getInstance()->useTextualGrammar())
	{
		TextualGrammarWriter textualGrammarWriter(dagGrammar, mainGrammar,
				pFilePath);
	}

	if (TreeRePairMode::getInstance()->isPrintBplexLike())
	{
		BplexGrammarWriter bplexGrammarWriter(dagGrammar, mainGrammar,
				pFilePath);
	}

#if !defined(__GNUC__)
	clock_t finish = clock();
	double clockTicks = finish - start;
	double timeInMilliseconds = clockTicks / (CLOCKS_PER_SEC / 1000);
#else
	clock_gettime(CLOCK_REALTIME, &time2);
	timespec runtime = Helper::diff(time1, time2);
	double timeInMilliseconds = runtime.tv_nsec/1000000 + runtime.tv_sec*1000;
#endif

	writeStatistics((unsigned int) timeInMilliseconds, numberOfInputEdges);
}

TreeRePairAlgorithm::~TreeRePairAlgorithm(void)
{
	// cout << "Deallocating memory ..." << endl;

	HASH_D_D::iterator it;
	for (it = pDigramHashMap->begin(); it != pDigramHashMap->end();)
	{
		Digram * pDigram = it->second;
		++it;
		delete pDigram;
		pDigram = 0;
	}

	delete pPriorityQueue;
	delete pDigramHashMap;

	Helper::deleteSymbols();

	delete pDigramCounter;

	TreeRePairMode::deleteInstance();
}

void TreeRePairAlgorithm::writeStatistics(unsigned int timeInMilliseconds,
		unsigned int numberOfInputEdges)
{
	if (TreeRePairMode::getInstance()->isVerboseMode()
			|| TreeRePairMode::getInstance()->isShowStatistics()
			|| TreeRePairMode::getInstance()->writeBinaryStatistics())
	{

		Statistics statistics(pFilePath, dagGrammar, timeInMilliseconds,
				numberOfInputEdges, mainGrammar, Helper::getCurrentMode().c_str());

		// write statistics to file for documentation purposes
		// statistics.writeStatisticsFile();

		if (TreeRePairMode::getInstance()->writeBinaryStatistics())
		{
			string fileNameStatisticsBinary =
					Helper::getFilePathForStatisticsBinary(pFilePath);
			ofstream foutStatisticsBinary(fileNameStatisticsBinary.c_str(),
					ios::out | ios::binary);
			// foutStatisticsBinary.write((char*) &statistics, sizeof(Statistics));
			statistics.serialize(foutStatisticsBinary);
			foutStatisticsBinary.close();
		}

		// write statistics to standard output too
		if (TreeRePairMode::getInstance()->isVerboseMode()
				|| TreeRePairMode::getInstance()->isShowStatistics())
		{
			statistics.writeStatistics(cout);
		}
	}
}

void TreeRePairAlgorithm::parseInput()
{

	XMLTreeParser xmlParser(pFilePath, dagGrammar);
	nodeCount = xmlParser.getNodeCount();
}

void TreeRePairAlgorithm::pruneGrammar()
{
	if (mainGrammar.size() == 0) {
		return;
	}

#ifdef DEBUG
	cout << "first test of validity" << endl;
	Helper::areGrammarsValid(dagGrammar, mainGrammar, pFilePath);
#endif

	Production * pProduction = dagGrammar[0];
	ReferencesTreeCounter treeCounter(pProduction->getRightHandSide(), &dagGrammar, mainGrammar);

	for (unsigned int i = 0; i < mainGrammar.size(); i++) {
		pProduction = mainGrammar[i];
		ReferencesTreeCounter treeCounter(pProduction->getRightHandSide(), 0, mainGrammar);
	}

	GrammarPruner pruner(dagGrammar, mainGrammar, false);
#ifdef DEBUG
	pruner.setFilePath(pFilePath);
#endif
	pruner.pruneGrammar();
}

void TreeRePairAlgorithm::pruneDagGrammar()
{
	if (TreeRePairMode::getInstance()->isVerboseMode()) {
		EdgeCounter edgeCounter;
		edgeCounter.countEdges(dagGrammar);
			cout << "Binary DAG consists of " << edgeCounter.getNumberOfEdges()
			<< " edges and " << dagGrammar.size()
			<< " non-terminals." << endl;
	}

	if (TreeRePairMode::getInstance()->isVerboseMode()) {
		cout << "Removing non-terminals referenced only once ..." << endl;
	}

	GrammarPruner grammarPruner(dagGrammar, mainGrammar, true);
	grammarPruner.pruneGrammar();

	if (TreeRePairMode::getInstance()->isVerboseMode()) {
		EdgeCounter edgeCounter;
		edgeCounter.countEdges(dagGrammar);
			cout << "Binary DAG consists of " << edgeCounter.getNumberOfEdges()
			<< " edges and " << dagGrammar.size()
			<< " non-terminals." << endl;
	}
}
