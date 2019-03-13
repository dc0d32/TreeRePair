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

#include "DigramPriorityQueue.h"
#include "TreeNode.h"
#include "TreeRePairMacros.h"
#include "Grammar.h"

#include <string>
#include <sstream>

#if !defined(__GNUC__)
#include <algorithm>
#else
#include <limits.h>
#endif

#include <time.h>

using namespace std;

/**
 * This class contains various static helper methods.
 */
class Helper {
public:

#if defined(__GNUC__)
	/**
	 * Computes the difference between two timestamps.
	 */
	static timespec diff(timespec start, timespec end);
#endif

	/**
	 * Computes the file size of the specfied file.
	 * @param szFileName the file of which the size shall be computed
	 * @returns the size of the file
	 */
	static int getFileSize(const char * szFileName);

	/**
	 * This method takes the binary representation of the input tree
	 * and writes it to a output file. In particular, it converts
	 * the tree on-the-fly to its unranked form.
	 * @param pNode the binary representation of the input tree
	 * @param pFilePath the file to write to
	 */
	static void writeUncompressedFile(TreeNode * pNode, const char * pFilePath);

#if __GNUC__
	/**
	 * This program executes a command and returns the output generated
	 * by the command.
	 * @param executeStr the command to execute
	 *
	 */
	static string parseProgramOutputUnderLinux(string executeStr);

	/**
	 * Checks if two files differ using Linux' diff command. This method is only
	 * for debugging purposes.
	 * @returns true if the specified files differ
	 */
	static bool doFilesDiffer(string strFileA, string strFileB);
#endif

	/**
	 * Retrieves for a given tree node and a child index the corresponding
	 * digram from the digram hashmap.
	 * @param pDigramHashMap the digram hash map to use
	 * @param dagGrammar the grammar which is needed to substitute nodes labeled
	 *  by nonterminals by the corresponding right-hand side
	 * @param pNode the node whose label will serve as the parent symbol of the
	 *  digram to be retrieved
	 * @param childIndex the index which indirectly specifies the child symbol of
	 *  the digram to be retrieved
	 * @returns the digram of which the specified node is one occurrence
	 */
	static Digram * getDigram(HASH_D_D * pDigramHashMap, Grammar & dagGrammar, TreeNode * pNode, unsigned int childIndex);

	/**
	 * Returns the current mode of the algorithm in the form of a human
	 * readable string.
	 * @returns a human readable mode specifier
	 */
	static string getCurrentMode();

	/**
	 * Prints the current mode to an output stream.
	 * @param out the output stream to use (cout is default)
	 */
	static void printCurrentMode(ostream & out = cout);

	/**
	 * Prints a progress bar indicating the progress of a digram replacement.
	 * @param current the number of occurrence replacements done so far
	 * @param max the total number of occurrence replacements which need to
	 *  be executed
	 */
	static void printProgress(size_t current, size_t max);

	/**
	 * Returns a string consisting of blank spaces.
	 * @param depth the number of blank spaces (i.e., the length of the
	 *  string returned)
	 * @returns a string solely consisting of blank spaces
	 */
	static string getBlankCharacters(int depth);

	/**
	 * Deletes all instances of class Symbol.
	 */
	static void deleteSymbols();

	/**
	 * Returns the file name which is used to write the decompression
	 * result to.
	 * @param pFilePath the name of the compressed file
	 * @return the file name of the decompressed file
	 */
	static string getFilePathForUncompressed(const char * pFilePath);

	/**
	 * Returns the name of the file which contains only the XML document tree
	 * of the specified XML document.
	 * @param pFilePath the name of the XML file
	 * @return the name of the file containing only the XML document tree
	 */
	static string getFilePathForStructure(const char * pFilePath);

	/**
	 * Returns the name of the file which is used to write a human readable
	 * form of the statistics of a run of TreeRePair to.
	 * @param pFilePath the name of the input XML document
	 * @returns the name of the statistics file
	 */
	static string getFilePathForStatistics(const char * pFilePath);

	/**
	 * Returns the name of the file which is used to write a binary
	 * representation of the statistics of a run of TreeRePair to.
	 * @param pFilePath the name of the input XML document
	 * @returns the name of the binary statistics file
	 */
	static string getFilePathForStatisticsBinary(
			const char * pFilePath);

	/**
	 * Returns the name of the file which is used to write the final
	 * grammar in a BPLEX-like format to.
	 * @param pFilePath the name of the input XML document
	 * @returns the name of the BPLEX-like grammar file
	*/
	static string getFilePathForBplexLike(const char * pFilePath);

	/**
	 * Returns the name of the file which is used to write the final
	 * grammar in a human readable format to.
	 * @param pFilePath the name of the input XML document
	 * @returns the name of the textual grammar file
	*/
	static string getFilePathForTextual(const char * pFilePath);

	/**
	 * Returns the name of the file which is used to write the final
	 * grammar in its binary representation to (i.e., after the
	 * succinct coding has been applied to the grammar).
	 * @param pFilePath the name of the input XML document
	 * @returns the name of the file containing the succinct coding
	*/
	static string getFilePathForBinary(const char * pFilePath);

	/**
	 * Returns a string containing the specified decimal value.
	 */
	static string stringify(size_t x);

	/**
	 * Returns a string containing the specified decimal value.
	 */
	static string stringify(int x);

	// static string stringify(unsigned int x);

	/**
	 * Returns a string containing the specified double value.
	 */
	static string stringify(double x);

	/**
	 * Rounds the specified value with double precision in order
	 * to obtain an integer.
	 */
	static int round(double x);

	/**
	 * Formats time in milliseconds to string.
	 * @param timeInMilliseconds the time to format in milliseconds
	 * @returns the formatted string
	 */
	static string formatTimeInMilliseconds(int timeInMilliseconds);

	/**
	 * Formats memory usage to string.
	 * @param memInKbs memory in kilobytes
	 * qreturns the formatted string
	 */
	static string formatMemSizeInMB(int memInKbs);

	/**
	 * Returns either a new line control character or an
	 * empty string.
	 * @param useFormatting if true, a new line control character is
	 *  returned; otherwise an empty string is returned
	 */
	static string getEndLine(bool useFormatting);

	/**
	 * Trims a string by removing a specified string from its
	 * beginning and ending.
	 * @param s the string to trim
	 * @param drop the string to remove from the specified string
	 * @returns the trimmed string
	 */
	static string trim(const std::string& s, const std::string& drop = " ");

	/**
	 * Checks if a file exists.
	 * @param fileName the file to check
	 * @returns true if the file exists; otherwise false
	 */
	static bool fileExists(const char * fileName);

	/**
	 * Returns the name of a file without its extension.
	 */
	static string getFileNamePrefix(const char * pFilePath);

	/**
	 * Returns the path to a file (without the file name itself).
	 */
	static string getPath(const string & fullFilename);

#ifdef DEBUG
	// for debugging purposes only

	/**
	 * Checks whether the current state of the DAG grammar and the
	 * grammar which results from the digram replacements are valid,
	 * i.e., if they really generate the input tree. This method is
	 * only for debugging purposes and is expensive to execute.
	 */
	static bool areGrammarsValid(Grammar & dagGrammar,
			Grammar & mainGrammar, char * pFilePath);

	static bool checkForWellFormedTree(const string &strTree);
	static bool isValidOccurrence(Grammar & grammar,
			TreeNode * pOccurrence, Digram * pDigram, int counter);
	static void printTree(TreeNode * pRoot, ostream & out = cout,
			unsigned int maxDepth = INT_MAX);
	static void printTreeAux(TreeNode * pNode,
			unsigned int indention, ostream & out, unsigned int maxDepth);
	static void parseForInvalidSymbols();
	static void printDigramsHashMap(HASH_D_D * pDigramsHashMap);
	static void printPriorityQueue(DigramPriorityQueue * pPriorityQueue);
#endif

private:
	/**
	 * Substitutes the extension of a given file by the specified one
	 * (default is .xml). Optionally a second string is added right after
	 * the file name before the extension.
	 * @param pFilePath the base file
	 * @param desc the optional string appended to the file name just
	 *  before the extension
	 * @param type the extension to use (.xml is the default)
	 * @returns the altered file name
	 */
	static string getFilePath(const char * pFilePath, const string &desc,
			string type = "xml");

	/**
	 * Returns the prefix of a file name, i.e., the file name without
	 * the extension specifier specified (and the dot).
	 * @param filename the file name whose prefix shall be returned
	 * @param suffix the suffix to remove from the file name
	 * @returns the file name without the specified extension specifier
	 */
	static string getFileNamePrefixAux(const string &filename,
			const string &suffix);
};
