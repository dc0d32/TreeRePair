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
#include "Statistics.h"

#include "EdgeCounter.h"
#include "Helper.h"
#include "Production.h"

#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sys/types.h>

Statistics::Statistics(const char * pFilePathPar,
		Grammar & dagGrammar, int timeInSecondsPar,
		int numberOfInputEdgesPar, Grammar & mainGrammar, string modePar) :
	pFilePath(pFilePathPar), timeInSeconds(timeInSecondsPar),
	numberOfInputEdges(numberOfInputEdgesPar), mode(modePar), memVmRSS(0)
{
	EdgeCounter edgeCounter(&dagGrammar);
	edgeCounter.countEdges(dagGrammar[0]->getRightHandSide());
	edgeCounter.countEdges(mainGrammar);

	edgesOutputSize = edgeCounter.getNumberOfEdges();

	computeEdgesCompression();

	computeFilesystemOutputSize();
	computeFilesystemInputSize();
	computeFilesystemCompression();

	numberOfNonTerminals = mainGrammar.size() + 1;

	computeInitialSize(dagGrammar);
	computeInitialSizeFactor();
}

Statistics::Statistics(int filesystemInputSizePar, int filesystemOutputSizePar,
		int timeInSecondsPar, int numberOfOutputEdgesPar,
		int numberOfInputEdgesPar, int numberOfNonTerminalsPar, string modePar, int memVmRSSPar) :
	timeInSeconds(timeInSecondsPar), numberOfInputEdges(numberOfInputEdgesPar),
			edgesOutputSize(numberOfOutputEdgesPar), numberOfNonTerminals(
					numberOfNonTerminalsPar), filesystemInputSize(
					filesystemInputSizePar), filesystemOutputSize(
					filesystemOutputSizePar), initialSize(0),
			initialSizeFactor(0.0), mode(modePar), memVmRSS(memVmRSSPar)
{
	computeEdgesCompression();
	computeFilesystemCompression();

	// calculateInitialSize();
	// calculateInitialSizeFactor();
}

Statistics::Statistics(const char * pFilePathPar, int timeInSecondsPar,
		int numberOfOutputEdgesPar, int numberOfInputEdgesPar,
		int numberOfNonTerminalsPar, string modePar, int memVmRSSPar) :
	pFilePath(pFilePathPar), timeInSeconds(timeInSecondsPar),
			numberOfInputEdges(numberOfInputEdgesPar), edgesOutputSize(
					numberOfOutputEdgesPar), numberOfNonTerminals(
					numberOfNonTerminalsPar), initialSize(0),
			initialSizeFactor(0.0), mode(modePar), memVmRSS(memVmRSSPar)
{
	computeEdgesCompression();

	computeFilesystemOutputSize();
	computeFilesystemInputSize();
	computeFilesystemCompression();

	// calculateInitialSize();
	// calculateInitialSizeFactor();
}

Statistics::Statistics(void)
{
}

Statistics::~Statistics(void)
{
}

void Statistics::computeFilesystemOutputSize()
{
	string pOutputFilePath = Helper::getFilePathForBinary(pFilePath).c_str();
	filesystemOutputSize = Helper::getFileSize(pOutputFilePath.c_str());
}

void Statistics::computeFilesystemInputSize()
{
	filesystemInputSize = Helper::getFileSize(pFilePath);
}

void Statistics::computeInitialSize(Grammar & dagGrammar)
{
	EdgeCounter edgeCounter(&dagGrammar);
	edgeCounter.countEdges(dagGrammar[0]->getRightHandSide());
	initialSize = edgeCounter.getNumberOfEdges();
}

void Statistics::computeEdgesCompression()
{
	if (numberOfInputEdges == 0) {
		edgesCompression = 0;
		return;
	}

	edgesCompression = ((double) edgesOutputSize)
			/ (((double) numberOfInputEdges) / 100);
}

void Statistics::computeFilesystemCompression()
{
	filesystemCompression = ((double) filesystemOutputSize)
			/ (((double) filesystemInputSize) / 100);
}

void Statistics::computeInitialSizeFactor()
{
	initialSizeFactor = ((double) initialSize)
			/ (((double) getEdgesOutputSize()) / 100);
}

int Statistics::getRunTime()
{
	return timeInSeconds;
}

void Statistics::writeStatistics(ostream & out)
{
	out << setprecision(3);

	out << "#Edges: Input: " << getEdgesInputSize() << ", Output: "
			<< getEdgesOutputSize() << ", Ratio: " << getEdgesCompression()
			<< "%" << endl;

	out << "File size: Input: " << getFilesystemInputSize() << ", Output: "
			<< getFilesystemOutputSize() << ", Ratio: "
			<< getFilesystemCompression() << "%" << endl;

	out << "#Non-Terminals: " << getNumberOfNonTerminals() << ", ";
	out << "Size of start production: " << getInitialSize() << " edges, " << endl;
	out << "Size of start production/|G|: " << getInitialSizeFactor() << "%";
	out << endl;

	out << "Total time needed: " << Helper::formatTimeInMilliseconds(
			getRunTime()) << endl;

}

void Statistics::serialize(ofstream & arc)
{
	 int size = mode.size();
	 arc.write(reinterpret_cast<char *>(&size),sizeof(size));
	 arc.write(mode.c_str(), size+1); // write final '\0' too

	 arc.write(reinterpret_cast<char *>(&timeInSeconds), sizeof(timeInSeconds));

	 arc.write(reinterpret_cast<char *>(&numberOfInputEdges), sizeof(numberOfInputEdges));

	 arc.write(reinterpret_cast<char *>(&numberOfTerminals), sizeof(numberOfTerminals));

	 arc.write(reinterpret_cast<char *>(&edgesOutputSize), sizeof(edgesOutputSize));

	 arc.write(reinterpret_cast<char *>(&numberOfNonTerminals), sizeof(numberOfNonTerminals));

	 arc.write(reinterpret_cast<char *>(&filesystemInputSize), sizeof(filesystemInputSize));

	 arc.write(reinterpret_cast<char *>(&filesystemOutputSize), sizeof(filesystemOutputSize));

	 arc.write(reinterpret_cast<char *>(&initialSize), sizeof(initialSize));

	 arc.write(reinterpret_cast<char *>(&edgesCompression), sizeof(edgesCompression));

	 arc.write(reinterpret_cast<char *>(&filesystemCompression), sizeof(filesystemCompression));

	 arc.write(reinterpret_cast<char *>(&initialSizeFactor), sizeof(initialSizeFactor));

	 arc.write(reinterpret_cast<char *>(&memVmRSS), sizeof(memVmRSS));
}

void Statistics::deserialize(ifstream & arc)
{
	int size = 0;
	char *p = 0;
	arc.read(reinterpret_cast<char *>(&size), sizeof(size));
	p = new char [size+1];
	arc.read(p, size+1);
	mode = p;
	delete[] p;

	arc.read(reinterpret_cast<char *>(&timeInSeconds), sizeof(timeInSeconds));

	arc.read(reinterpret_cast<char *>(&numberOfInputEdges), sizeof(numberOfInputEdges));

	arc.read(reinterpret_cast<char *>(&numberOfTerminals), sizeof(numberOfTerminals));

	arc.read(reinterpret_cast<char *>(&edgesOutputSize), sizeof(edgesOutputSize));

	arc.read(reinterpret_cast<char *>(&numberOfNonTerminals), sizeof(numberOfNonTerminals));

	arc.read(reinterpret_cast<char *>(&filesystemInputSize), sizeof(filesystemInputSize));

	arc.read(reinterpret_cast<char *>(&filesystemOutputSize), sizeof(filesystemOutputSize));

	arc.read(reinterpret_cast<char *>(&initialSize), sizeof(initialSize));

	arc.read(reinterpret_cast<char *>(&edgesCompression), sizeof(edgesCompression));

	arc.read(reinterpret_cast<char *>(&filesystemCompression), sizeof(filesystemCompression));

	arc.read(reinterpret_cast<char *>(&initialSizeFactor), sizeof(initialSizeFactor));

	arc.read(reinterpret_cast<char *>(&memVmRSS), sizeof(memVmRSS));
}

void Statistics::writeStatisticsFile()
{

	string filename = Helper::getFilePathForStatistics(pFilePath);

	// cout << "Writing file " << filename << " ..." << endl;

	ofstream fout(filename.c_str());

	Helper::printCurrentMode(fout);
	writeStatistics(fout);

	fout.close();

}
