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
#include "Helper.h"

#include "Digram.h"
#include "TreeRePairMode.h"
#include "TreeRePairConstants.h"
#include "TreeRePairMacros.h"
#include "SymbolInfoContainer.h"
#include "SymbolInfoContainerHasher.h"
#include "XmlTreePrinter.h"

#ifdef DEBUG
#include "SymbolReplacer.h"
#include "TreeDeleter.h"
#include "Production.h"
#include "TreeCopier.h"
#endif

#include <sstream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <sys/stat.h>

#if !defined(__GNUC__)
#include <limits>
#else
#include <limits.h>
#endif

#define SETSIZE 4

#if defined(__GNUC__)
// from: http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
timespec Helper::diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
#endif

#ifdef DEBUG
bool Helper::areGrammarsValid(Grammar & dagGrammarPar,
		Grammar & mainGrammarPar, char * pFilePath) {

	Grammar & dagGrammar = *dagGrammarPar.copy();
	Grammar & mainGrammar = *mainGrammarPar.copy();
//	Grammar & dagGrammar = dagGrammarPar;
//	Grammar & mainGrammar = mainGrammarPar;

	Production * pRootProduction = dagGrammar[0];

	TreeCopier copier(&dagGrammar, pRootProduction->getRightHandSide());

	TreeNode * pOldRightHandSide = pRootProduction->getRightHandSide();
	dagGrammar.updateRightHandSide(pRootProduction, copier.getNewRoot());
	// TODO delete other dag productions
 	TreeDeleter treeDeleter;
 	treeDeleter.deleteTree(pOldRightHandSide);

	SymbolReplacer symbolReplacer(dagGrammar, mainGrammar);
	SymbolReplacer::replaceNonterminals(symbolReplacer, mainGrammar);

	cout << "Converting binary tree to unranked tree ..." << endl;

	pRootProduction = dagGrammar[0];
//	TreeConverter treeConverter(pRootProduction->getRightHandSide());

	writeUncompressedFile(dagGrammar[0]->getRightHandSide(), pFilePath);
//	writeUncompressedFile(treeConverter.getNewRoot(), pFilePath);

	return !doFilesDiffer(string(pFilePath), getFilePathForUncompressed(pFilePath));
}
#endif

#if __GNUC__
bool Helper::doFilesDiffer(string strFileA, string strFileB) {
	string executeStr = "diff -q " + strFileA + " " + strFileB;
	string output = parseProgramOutputUnderLinux(executeStr);

//	cout << "diff: " << output << endl;

	size_t pos = output.find_first_of("differ", 0);
	if (pos != string::npos) {
		return true;
	}
	return false;
}
#endif

int Helper::getFileSize(const char * szFileName)
{
	struct stat fileStat;
	int err = stat(szFileName, &fileStat);
	if (0 != err)
		return 0;
	return fileStat.st_size;
}

void Helper::writeUncompressedFile(TreeNode * pNode, const char * pFilePath)
{
	string filename = Helper::getFilePathForUncompressed(pFilePath);

	cout << "Writing file " << filename << " ..." << endl;

	ofstream fout(filename.c_str());
	XmlTreePrinter xmlTreePrinter(fout);
//	xmlTreePrinter.printTree(pNode, false, false);
	xmlTreePrinter.printTree(pNode, false, true);
	fout.close();
}

#if __GNUC__
string Helper::parseProgramOutputUnderLinux(string executeStr)
{

	FILE * programOutput = popen(executeStr.c_str(), "r");
	if (!programOutput)
	{
		cout << "Fatal error: Couldn't parse output of program. Exiting ..."
				<< endl;
		exit(EXIT_FAILURE);
	}

	string output = "";
	char buf[1000];
	string currentString;

	while (fgets(buf, sizeof(buf), programOutput))
	{
		currentString = buf;
		output += currentString;
	}

	pclose(programOutput);

	return output;

}
#endif

Digram * Helper::getDigram(HASH_D_D * pDigramHashMap, Grammar & dagGrammar, TreeNode * pNode, unsigned int childIndex) {

	TreeNode * pChild = TreeNode::getChild(dagGrammar, pNode, childIndex);

	Digram * pDigram = new Digram(childIndex, pNode->getSymbol(), pChild->getSymbol(), pNode, pNode);

	HASH_D_D::iterator it = pDigramHashMap->find(pDigram);
	if (it != pDigramHashMap->end()) {
		delete pDigram;
		pDigram = it->second;
	} else {
		delete pDigram;
		pDigram = 0;
	}
	return pDigram;
}

void Helper::printProgress(size_t current, size_t max)
{
	if (max == 0) {
		cout << "Error: Cannot calculate progress information." << endl;
	}

	double step = ((double) max) / ((double) PROGRESS_RESOLUTION);
	int currentStep = round(((double) current) / step);

	cout << "\r[";

	for (int i = 0; i < currentStep; i++)
	{
		cout << "#";
	}

	for (int i = currentStep; i < PROGRESS_RESOLUTION; i++)
	{
		 cout << ".";
	}

	cout << "] (" << current << "/" << max << ")";
	cout << flush;
}

string Helper::formatTimeInMilliseconds(int timeInMilliseconds)
{

	string str;

	if (timeInMilliseconds < 1000)
	{
		str = Helper::stringify(timeInMilliseconds) + "ms";
	}
	else if (timeInMilliseconds < 10000)
	{
		double timeInSeconds = ((double) timeInMilliseconds) / (double) 1000;
		ostringstream temp;
		temp << fixed << setprecision(3) << timeInSeconds << "s";
		str = temp.str();
	}
	else
	{
		int timeInSeconds = timeInMilliseconds / 1000;
		if (timeInSeconds < 60)
		{
			str = Helper::stringify(timeInSeconds) + "s";
		}
		else
		{
			int minutes = (int) floor((double) timeInSeconds / 60.0);
			int seconds = timeInSeconds % 60;
			str = Helper::stringify(minutes) + "m "
					+ Helper::stringify(seconds) + "s";
		}
	}

	return str;
}

string Helper::formatMemSizeInMB(int memInKBs)
{

	string str;

//	if (memInKBs < 1024)
//	{
//		str = "0." + Helper::stringify(memInKBs);
//	} else
	if (memInKBs < 10 * 1024) {
		double dblMemInKbs = memInKBs;
		double dblMemInMbs = dblMemInKbs / 1024;

		std::ostringstream o;
		o << fixed << setprecision(3);
		o << dblMemInMbs;
		str = o.str();
	} else {
		str = Helper::stringify(memInKBs);
	}

	return str;
}

int Helper::round(double x)
{
	return (int) (((x - floor(x)) >= 0.5) ? ceil(x) : floor(x));
}

#ifdef DEBUG
bool Helper::checkForWellFormedTree(const string &strTree)
{

	int counter = 0;

	for (string::size_type i = 0; i < strTree.length(); i++)
	{
		if (strTree.at(i) == '(')
		{
			counter++;
		}
		else if (strTree.at(i) == ')')
		{
			counter--;
		}

		if (counter < 0)
		{
			return false;
		}
	}

	if (counter == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}
#endif

void Helper::printCurrentMode(ostream & out)
{

	out << "Mode is \"" << getCurrentMode() << "\"";

	out << endl;
}

string Helper::getCurrentMode()
{

	string mode = "";

	if (TreeRePairMode::getInstance()->getOptimizationMode()
			== TreeRePairMode::OPTIMIZE_FILE_SIZE)
	{
		mode += "file size";
	}
	else if (TreeRePairMode::getInstance()->getOptimizationMode()
			== TreeRePairMode::OPTIMIZE_EDGES)
	{
		mode += "edges";
	}
	else if (TreeRePairMode::getInstance()->getOptimizationMode()
			== TreeRePairMode::OPTIMIZE_DISABLE)
	{
		mode += "no pruning";
	}
	else
	{
		mode += "unknown optimization mode";
	}

	if (TreeRePairMode::getInstance()->isMaxRankSet()) {
		int maxRank = TreeRePairMode::getInstance()->getMaxRank();
		mode += ", max rank: " + Helper::stringify(maxRank);
	}

	if (!TreeRePairMode::getInstance()->isUseDag()) {
		mode += ", no dag";
	}

#ifdef ENVIRONMENT64
	mode += ", 64 bit";
#else
	mode += ", 32 bit";
#endif

#ifdef DEBUG
	mode += ", DEBUG";
#endif

	return mode;

}

#ifdef DEBUG
void Helper::printDigramsHashMap(HASH_D_D * pPairHashMap)
{
	// Listing the digrams key, value (they're not ordered.)
	HASH_D_D::iterator it;
	Digram * pDigram = 0;
	for (it = pPairHashMap->begin(); it != pPairHashMap->end(); ++it)
	{
		// std::cout << "Key = \'" << it->first << "\' -> value = \'";
		pDigram = it->second;
		Digram::print(cout, pDigram);
		cout << ", " << pDigram->getNumberOfOccs() << " occurrences";
		cout << endl;
	}
}

void Helper::printPriorityQueue(DigramPriorityQueue * pPriorityQueue)
{
	DigramPriorityQueue::print(cout, pPriorityQueue);
}
#endif

string Helper::getFileNamePrefix(const char * pFilePath)
{
	string filename(pFilePath);

	filename = getFileNamePrefixAux(filename, string(".txt"));
	filename = getFileNamePrefixAux(filename, string(".xml"));

	return filename;
}

string Helper::getFileNamePrefixAux(const string &filename,
		const string &suffix)
{

	string prefix;
	size_t index = filename.find_last_of(suffix);
	if (index == (filename.length() - 1))
	{
		prefix = filename.substr(0, filename.length() - 4);
	}
	else
	{
		prefix = filename;
	}

	return prefix;
}

string Helper::getPath(const string & fullFilename)
{
	size_t index = fullFilename.find_last_of("/");

	if (index == string::npos) {
		index = fullFilename.find_last_of("\\");
	}

	string path = fullFilename.substr(0, index);

	return path;
}

string Helper::getFilePathForBinary(const char * pFilePath)
{
	if (TreeRePairMode::getInstance()->getAlgMode()
			== TreeRePairMode::TOOL_BATCH_BPLEX)
	{
		return getPath(string(pFilePath)) + "/multiplex.succ68";
	}
	else
	{
		// return getFilePath(pFilePath, string("compressed"), string("bin"));
		return getFilePath(pFilePath, string(""), string("rep"));
	}
}

string Helper::getFilePathForTextual(const char * pFilePath)
{
	return getFilePath(pFilePath, string(""), string("reptxt"));
}

string Helper::getFilePathForUncompressed(const char * pFilePath)
{
	return getFilePath(pFilePath, string("uncompressed"));
}

string Helper::getFilePathForStructure(const char * pFilePath)
{

	string extension = "structure.xml";
	string filePath(pFilePath);

	// check if we are already working on a structure file
	if (filePath.substr(filePath.length() - extension.length(),
			extension.length()) == extension)
	{
		// cout << "is structure file" << endl;
		return filePath;
	}

	return getFilePath(pFilePath, string("structure"));
}

string Helper::getFilePathForStatistics(const char * pFilePath)
{
	string filePath = getFilePath(pFilePath, string("statistics"),
			string("txt"));
	return filePath;
}

string Helper::getFilePathForStatisticsBinary(const char * pFilePath)
{
	return getFilePath(pFilePath, string("statistics"), string("bin"));
}

string Helper::getFilePathForBplexLike(const char * pFilePath)
{
	return getFilePath(pFilePath, string(""), string("g"));
}

string Helper::getFilePath(const char * pFilePath, const string &desc,
		string type) {
	string filename = getFileNamePrefix(pFilePath);

	if (desc.size() > 0) {
		filename += "_";
		filename += desc;
	}

	if (type.length() > 0)
	{
		filename += ".";
		filename += type;
	}
	return filename;
}

#ifdef DEBUG
void Helper::parseForInvalidSymbols()
{
	HASH_S_S::iterator it;
	for (it = Symbol::hashMaps.symbolsBySicHashMap.begin(); it
			!= Symbol::hashMaps.symbolsBySicHashMap.end(); ++it)
	{
		Symbol * pSymbol = it->second;
		if (pSymbol->getRank() > TreeRePairMode::getInstance()->getMaxRank())
		{
			cout << "Fatal error: Symbol " << Symbol::getElementTypeById(
					pSymbol->getId()) << " has arity " << pSymbol->getRank()
					<< " which is bigger than the allowed bound of "
					<< TreeRePairMode::getInstance()->getMaxRank() << "."
					<< endl;
			exit(1);
		}
	}
}
#endif

void Helper::deleteSymbols()
{
	HASH_S_S::iterator it, itTemp;
	for (it = Symbol::hashMaps.symbolsBySicHashMap.begin(); it
			!= Symbol::hashMaps.symbolsBySicHashMap.end();)
	{
		itTemp = it;
		it++;
		delete itTemp->first;
		// it->first = 0;
	}
	Symbol::hashMaps.symbolsBySicHashMap.clear();

	HASH_U_S::iterator it2;
	for (it2 = Symbol::hashMaps.symbolByIdHashMap.begin(); it2
			!= Symbol::hashMaps.symbolByIdHashMap.end(); ++it2)
	{
		delete it2->second;
		it2->second = 0;
	}
}

string Helper::getEndLine(bool useFormatting)
{

	std::ostringstream o;
	o << "";

	if (useFormatting)
	{
		o << endl;
	}

	return o.str();
}

string Helper::getBlankCharacters(int depth)
{
	string blanks = "";
	for (int i = 0; i < depth; i++)
	{
		blanks += "  ";
	}
	return blanks;
}

string Helper::stringify(size_t x)
{
	std::ostringstream o;
	o << x;
	return o.str();
}

string Helper::stringify(int x)
{
	std::ostringstream o;
	o << x;
	return o.str();
}

//string Helper::stringify(unsigned int x)
//{
//	std::ostringstream o;
//	o << x;
//	return o.str();
//}

string Helper::stringify(double x)
{
	std::ostringstream o;
	o << fixed << setprecision(3);
	o << x;
	return o.str();
}

string Helper::trim(const std::string& s, const std::string& drop)
{
	std::string::size_type first = s.find_first_not_of(drop);
	std::string::size_type last = s.find_last_not_of(drop);

	if (first == std::string::npos || last == std::string::npos)
		return std::string("");
	return s.substr(first, last - first + 1);
}

bool Helper::fileExists(const char * fileName)
{
	std::fstream fin;
	fin.open(fileName, std::ios::in);
	if (fin.is_open())
	{
		fin.close();
		return true;
	}
	fin.close();
	return false;
}

#ifdef DEBUG
bool Helper::isValidOccurrence(Grammar & grammar,
		TreeNode * pOccurrence, Digram * pDigram, int counter)
{
	TreeNode * pChild = TreeNode::getChild(grammar, pOccurrence, pDigram->getChildIndex());
	if (pOccurrence->getSymbol() != pDigram->getParentSymbol()
			|| pChild->getSymbol()
					!= pDigram->getChildSymbol())
	{

		cout << "Fatal error when replacing occurrences - found digram ("
				<< Symbol::getElementTypeById(pOccurrence->getSymbol()->getId())
				<< ", " << Symbol::getElementTypeById(pChild->getSymbol()->getId())
				<< ") at occurrence number " << counter << ". Exiting ...";
		exit(1);
	}

	return true;
}
#endif
