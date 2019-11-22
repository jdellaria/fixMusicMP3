/*
 * configurationFile.h
 *
 *  Created on: Sep 10, 2009
 *      Author: jdellaria
 */

#ifndef CONFIGURATIONFILE_H_
#define CONFIGURATIONFILE_H_
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>


class configurationFile {
public:
	configurationFile();
	virtual ~configurationFile();

	void help(char *);
	int getConfiguration(char *);

	std::string logFileName;
	std::string logValue;
	bool logPrintScreen;
	bool logPrintFile;
	bool logPrintTime;
	std::string MP3OriginalDirectory; // Original MP3 Files should be placed here
	std::string MP3StagingDirectory; // Upon running prepareMusic files will be placed here
	std::string MP3MusicLibraryDirectory; // Upon running prepareMusic -l (Load files into the Music System) files will be placed here

};

#endif /* CONFIGURATIONFILE_H_ */
