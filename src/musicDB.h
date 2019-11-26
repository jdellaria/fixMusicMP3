/*
 * musicdb.h
 *
 *  Created on: Dec 26, 2008
 *      Author: jdellaria
 */

#ifndef MUSICDB_H_
#define MUSICDB_H_
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <tag.h>

class musicDB{
public:
//	char	name[255];
//	char	artist[150];
//	char	composer[150];
//	char	album[255];
//	char	grouping[100];
//	char	genre[100];

	int songID;
	std::string	name;
	std::string	artist;
	std::string	composer;
	std::string albumArtists;
	std::string	album;
	std::string	grouping;
	std::string	genre;
	std::string comments;
	long	trackNumber;
	long	diskNumber;
	long	diskCount;
	long	songYear;
	long	bitRate;
	long	sampleRate;
	long	songTime;
	char	dateModified[30];
	char	dateAdded[30];
	char	location[255];
//	std::string	location;
	long	artistId;
	long	albumId;
	char	thumblocation[255];

	MYSQL	dbaseConnection;
	MYSQL_RES *queryResult;


		musicDB();
		~musicDB();
	MYSQL	OpenConnection();
	void	CloseConnection();
	long	addAlbum();
	long	updateAlbumCover();
	long    updateSongComments();
	long	addSongToPreSongLibrary();
	long	addSongWithTimes( char * SQLStmt, MYSQL_TIME* dateModified, MYSQL_TIME* dateAdded);
	long	CommitSongsToLibrary();
	long	RemoveSongsFromPreSongLibrary();

	void	setArtist(std::string artistName);
	void    setArtist(char * myString);

	void	setArtistId(long id);
	void	setAlbum( std::string albumName);
	void    setAlbum(char * myString);


	void 	setComposer(std::string composerName);
	void    setComposer(char * myString);

	void 	setAlbumArtists(std::string albumArtistsName);
	void    setAlbumArtists(char * myString);

	void	setAlbumId(long id);
	void	setSongName(std::string songName);
	void    setSongName(char * myString);
	void	setGrouping(std::string groupingName);
	void	setGenre(std::string genreName);
	void    setGenre(char * myString);


	void	setDiskNumber(int snumber);
	void	setDiskNumber(char * myString);
	void 	setDiskCount(int snumber);
	void	setTrackNumber(int track);
	void	setTrackNumber(char * myString);
	void	setSongYear(int year);
	void	setBitRate(int brate);
	void	setSampleRate(int srate);
	void	setSongTime(int stime);
/*	void	setDateModified(char *date);
	void	setDateAdded(char *date);*/
	void	setLocation(char* where);
	void    setSongID(char * myString);
	void	setThumbLocation(char* where);
//	void	setLocation(TagLib::String where);
	long	addArtist();
	long	getArtistID();

	void    setComments(char * myString);

	void    setBitRate(char * myString);
	void    setSampleRate(char * myString);
	void    setSongTime(char * myString);
	void    setSongYear(char * myString);
	void    setArtistId(char * myString);
	void    setAlbumId(char * myString);

	MYSQL_RES * queryAlbumSongs();
	int  getNextSongRecord();
	int  mysqlFree();

};

#endif /* MUSICDB_H_ */
