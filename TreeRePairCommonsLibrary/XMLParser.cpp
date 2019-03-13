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
#include "XMLParser.h"

#include <iostream>
#include <fstream>

using namespace std;

XMLParser::XMLParser(char * pFilePath, XmlContentHandler & xmlContentHandlerPar) :
	xmlContentHandler(xmlContentHandlerPar)
{
	pIfs = new ifstream;
	pIfs->open(pFilePath);

	while (!pIfs->eof())
	{
		char buf = 0x00;
		pIfs->read(&buf, 1);

		if (buf == '<')
		{
			bool foundSlash = false;
			bool firstSymbol = true;
			bool nameComplete = false;
			bool skip = false;

			string name = "";
			while (!pIfs->eof() && !skip)
			{
				pIfs->read(&buf, 1);

				if (buf == '!' && firstSymbol)
				{
					pIfs->read(&buf, 1);

					if (buf == '-')
					{
						pIfs->read(&buf, 1);

						while (true)
						{
							pIfs->read(&buf, 1);
							if (buf == '-')
							{
								pIfs->read(&buf, 1);
								if (buf == '-')
								{
									pIfs->read(&buf, 1);
									if (buf == '>')
									{
										skip = true;
										break;
									}
								}
							}
						}
					} else {
						break;
					}
				}
				else if (buf == '?' && firstSymbol)
				{
					pIfs->read(&buf, 1);

					while (true)
					{
						pIfs->read(&buf, 1);
						if (buf == '?')
						{
							pIfs->read(&buf, 1);
							if (buf == '>')
							{
								skip = true;
								break;
							}
						}
					}
				}
				else if (buf != '>' && buf != '/' && buf != ' ' && buf != ':'
						&& !nameComplete)
				{
					name += buf;
				}
				else if (buf == ':' && !nameComplete)
				{
					name = "";
				}
				else if (buf == ' ')
				{
					nameComplete = true;
				}
				else if (buf == '>')
				{
					if (foundSlash)
					{
						xmlContentHandler.endElement(name);
					}
					else
					{
						xmlContentHandler.startElement(name);
					}
					break;

				}
				else if (buf == '/')
				{

					if (firstSymbol)
					{
						foundSlash = true;
					}
					else
					{
						pIfs->read(&buf, 1);
						if (buf == '>')
						{

							xmlContentHandler.startElement(name);
							xmlContentHandler.endElement(name);
							break;
						}
					}
				}

				firstSymbol = false;
			}
		}
	}

	xmlContentHandler.endDocument();
}

XMLParser::~XMLParser(void)
{
	pIfs->close();
	delete pIfs;
}
