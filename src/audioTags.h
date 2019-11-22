/*
 * audioTags.h
 *
 *  Created on: May 12, 2019
 *      Author: jdellaria
 */

#ifndef AUDIOTAGS_H_
#define AUDIOTAGS_H_

#include <tag.h>
class audioStringTags {
public:
	string utf8; // this is the default use fort C++ Strings. C2 and C3 character coding for characters > 0x7F ascii
	string ascii; // ISO Latin-1, Extended ascii. all characters fit into 1 byte
};

class audioTags {
public:
//	string title;
	audioStringTags title;
	audioStringTags artist;
	audioStringTags album;
	int year;
	int track;
	audioStringTags genre;
	audioStringTags location;
	audioStringTags composers;
	audioStringTags albumArtists;
	int bitrate;
	int sampleRate;
	int length;
	int disk;
	int diskcount;

	audioTags();
	virtual ~audioTags();
	int get(const char *  fileName);
	void getImage(const char * mp3FileName, const char * imageFileName);
	int set(const char *fileName);
};



#endif /* AUDIOTAGS_H_ */
