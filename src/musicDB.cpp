/*
 * musicdb.cpp
 *
 *  Created on: Dec 26, 2008
 *      Author: jdellaria
 */


#include "musicDB.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <locale.h>
#include <fcntl.h>
#include <time.h>

#include <iconv.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <iostream>
#include <string>

#define         MASKBITS                0x3F
#define         MASKBYTE                0x80
#define         MASK2BYTES              0xC0
#define         MASK3BYTES              0xE0
#define         MASK4BYTES              0xF0
#define         MASK5BYTES              0xF8
#define         MASK6BYTES              0xFC

typedef unsigned short   Unicode2Bytes;
typedef unsigned int     Unicode4Bytes;
typedef unsigned char    byte;

using namespace std;
extern DLog myLog;

musicDB::musicDB()
{
	OpenConnection();
//	name[0] = 0;
//	artist[0] = 0;
//	composer[0] = 0;
//	album[0] = 0;
//	grouping[0] = 0;
//	genre[0] = 0;
	trackNumber = 0;
	songYear = 0;
	dateModified[0] = 0;
	dateAdded[0] = 0;
	location[0] = 0;
	artistId = 0;
	albumId = 0;
	thumblocation[0] = 0;
	diskNumber = 0;
	diskCount = 0;
}


musicDB::~musicDB()
{
	CloseConnection();
}

void musicDB::setAlbum(string albumName)
{
	album = albumName;

}

void musicDB::setComposer(string composerName)
{
	composer = composerName;

}

void musicDB::setAlbumArtists(std::string albumArtistsName)
{
	albumArtists = albumArtistsName;
}


void musicDB::setAlbumId(long id)
{
	albumId = id;
}

void musicDB::setArtist(string artistName)
{
	artist = artistName;

}

void musicDB::setArtistId(long id)
{
	artistId = id;
}

void musicDB::setSongName(string songName)
{
	name = songName;

}

void musicDB::setGrouping(string groupingName)
{
	grouping = groupingName;

}

void musicDB::setGenre(string genreName)
{
	genre = genreName;
/*	for(int i = 0;i < genreName.length();i++)
	{
		genre[i] = genreName[i];
	}
	genre[genreName.length()] = 0;*/
}

void musicDB::setTrackNumber(int track)
{
	trackNumber = track;
}

void musicDB::setSongYear(int year)
{
	songYear = year;
}

void musicDB::setBitRate(int brate)
{
	bitRate = brate;
}

void musicDB::setSampleRate(int srate)
{
	sampleRate = srate;
}

void musicDB::setDiskNumber(int snumber)
{
	diskNumber = snumber;
}

void musicDB::setDiskCount(int snumber)
{
	diskCount = snumber;
}

void musicDB::setSongTime(int stime)
{
	songTime = stime;
}
/*
void musicDB::setDateModified(char *date)
{
	strcpy (dateModified, date);
}

*/
void musicDB::setLocation(char* where)
{
	int length = strlen(where);
//	for(int i = 0;i < where.length();i++)
	int j = 0;
	int i = 0;
	std::vector< unsigned char > g;

//This next section checks to see if we are give a UTF8 character (today we only handle C3). If it is, we then remove the C3 character and adjust the following character to the Windows 1252 character set.
	for (i=0; where[i]; i++) g.push_back(where[i]);
	for(i = 0;i < length;i++)
	{
//		if((g[i] & 0x80) == 0x80)
		if(g[i] == 0xC3)
		{
			i++;
			location[j] = where[i] | 0x40;
		}
		else
		{
			location[j] = where[i];
		}
		j++;
	}
	location[j] = 0;

}

void musicDB::setThumbLocation(char* where)
{
	int length = strlen(where);
//	for(int i = 0;i < where.length();i++)
	int j = 0;
	int i = 0;
	std::vector< unsigned char > g;

//This next section checks to see if we are give a UTF8 character (today we only handle C3). If it is, we then remove the C3 character and adjust the following character to the Windows 1252 character set.
	for (i=0; where[i]; i++) g.push_back(where[i]);
	for(i = 0;i < length;i++)
	{
//		if((g[i] & 0x80) == 0x80)
		if(g[i] == 0xC3)
		{
			i++;
			thumblocation[j] = where[i] | 0x40;
		}
		else
		{
			thumblocation[j] = where[i];
		}
		j++;
	}
	thumblocation[j] = 0;

}


//Music Database Connection
MYSQL musicDB::OpenConnection()
{
	int	err = 0;
	string message;
	/* now to connect to the database */

	if(mysql_init(&dbaseConnection) == NULL)
	{
		err = 1;
	}
	else
	{
		if(mysql_real_connect(&dbaseConnection,"localhost","root","dlirius","Music",0,NULL,0) == NULL)
			err = 1;
	}
	if(err)
	{
		message = "MusicDB.cpp ";
		message.append(__func__);
		message.append(": Failed to connect to database: ");
		message.append(mysql_error(&dbaseConnection));
		myLog.print(logError, message);
		printf("Failed to connect to database: Error: %s\n", mysql_error(&dbaseConnection));
		return(dbaseConnection);
	}
	return (dbaseConnection);
}

void musicDB::CloseConnection()
{
	mysql_close(&dbaseConnection);
}

long musicDB::addAlbum()	//album must be set before calling function
{
	char SQLStmt[1000];
	char tempAlbum[256];
	char tempArtist[256];
	char tempYear[256];
	long albumID;
	MYSQL_RES *queryResult;
	MYSQL_ROW row;
	int nrows;

//	strcpy (tempAlbum,album);
//	normalizeString(tempAlbum, 149);
	mysql_real_escape_string(&dbaseConnection, tempAlbum, album.c_str(), album.length());
	mysql_real_escape_string(&dbaseConnection, tempArtist, artist.c_str(), artist.length());

	sprintf(SQLStmt, "INSERT into Music.Albums (Album, refId, ArtistName, SongYear) values (TRIM('%s'),9998877,TRIM('%s'),%d);", tempAlbum, tempArtist, songYear); // this adds a new album with a unique ID (9998877) so that we can retrieve the albumID next
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " addAlbum\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}

	sprintf(SQLStmt, "SELECT AlbumID FROM Albums where refId = 9998877;");
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " addAlbum getting Album ID\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}
	queryResult = mysql_store_result(&dbaseConnection);
	nrows = mysql_num_rows(queryResult);
	if (nrows > 0)
	{
		row = mysql_fetch_row(queryResult);
		albumID = atol(row[0]);
	}
	sprintf(SQLStmt, "Update Albums set refId = 0 where refId = 9998877;");
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " addAlbum updating refId\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}

	return(albumID);
}


long musicDB::updateAlbumCover()	//album must be set before calling function
{
	char SQLStmt[1000];
	char tempthumblocation[256];

	mysql_real_escape_string(&dbaseConnection, tempthumblocation, thumblocation, strlen(thumblocation));

	sprintf(SQLStmt, "Update Albums set Cover = TRIM('%s') where AlbumId = %d;" ,tempthumblocation ,albumId);
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " updateAlbumCover updating Cover\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}

	return(albumId);
}


long musicDB::addArtist()	//artist must be set before calling function
{
	char SQLStmt[1000];
	char tempArtist[256];
	int length;

//	strcpy (tempArtist,artist);
//	normalizeString(tempArtist, 149);
	length = artist.length();
	if (length > 254)
		length = 254;
	mysql_real_escape_string(&dbaseConnection, tempArtist, artist.c_str(), length);
	artistId = getArtistID();
	if (artistId > 0)
	{
		return(artistId);
	}
	sprintf(SQLStmt, "INSERT into Music.Artists (Artist) values (TRIM('%s'));", tempArtist);
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " addArtist\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}
	artistId = getArtistID();
	return(artistId);
}

long musicDB::getArtistID()	//artist must be set before calling function
{
	char SQLStmt[1000];
	MYSQL_RES *queryResult;
	MYSQL_ROW row;
	int nrows;
	char tempArtist[256];
	int length;

//	strcpy (tempArtist,artist);
//	normalizeString(tempArtist, 149);
	length = artist.length();
	if (length > 254)
		length = 254;
	mysql_real_escape_string(&dbaseConnection, tempArtist, artist.c_str(), length);
	sprintf(SQLStmt, "SELECT ArtistID FROM Artists where Artist = TRIM('%s');", tempArtist);
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " getArtistID\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}
	queryResult = mysql_store_result(&dbaseConnection);
	nrows = mysql_num_rows(queryResult);
	if (nrows > 0)
	{
		row = mysql_fetch_row(queryResult);
		return (atol(row[0]));
	}
	return(0);
}


long musicDB::addSongToPreSongLibrary()
{
	char SQLStmt[2000];

	char tname[255];
	char tartist[150];
	char tcomposer[100];
	char talbumArtists[100];
	char talbum[150];
	char tgrouping[100];
	char tlocation[255];
	char tgenre[100];
	int length;

//	albumId = addAlbum();
//	artistId = addArtist();
//	cout << "musicDB::addSongToPreSongLibrary"  << endl;
	length = name.length();
	if (length > 254)
		length = 254;
	mysql_real_escape_string(&dbaseConnection, tname, name.c_str(), length);
	length = artist.length();
	if (length > 149)
		length = 149;
	mysql_real_escape_string(&dbaseConnection, tartist, artist.c_str(), length);
	length = album.length();
	if (length > 149)
		length = 149;
	mysql_real_escape_string(&dbaseConnection, talbum, album.c_str(), length);

	length = composer.length();
	if (length > 99)
		length = 99;
	mysql_real_escape_string(&dbaseConnection, tcomposer, composer.c_str(), length);
	length = albumArtists.length();
	if (length > 99)
		length = 99;
	mysql_real_escape_string(&dbaseConnection, talbumArtists, albumArtists.c_str(), length);

	length = grouping.length();
	if (length > 99)
		length = 99;
	mysql_real_escape_string(&dbaseConnection, tgrouping, grouping.c_str(), length);
	length = strlen(location);
	if (length > 254)
		length = 254;
	mysql_real_escape_string(&dbaseConnection, tlocation, location, length);
	length = genre.length();
	if (length > 99)
		length = 99;
	mysql_real_escape_string(&dbaseConnection, tgenre, genre.c_str(), length);

	if(diskNumber == 0) // if we have a disk number then add disk number and disk count
	{

		sprintf(SQLStmt, "INSERT into Music.presonglibrary (Name ,Artist, AlbumArtists ,Album, Composer, Grouping, SongYear, Location, TrackNumber, Genre, BitRate, SampleRate, SongTime, DateModified, DateAdded, AlbumId, ArtistId) values ('%s','%s','%s', '%s','%s', '%s' ,%d,'%s',%d, '%s' , %d, %d, %d, ?, ?,  %d, %d)", tname, tartist, talbumArtists, talbum, tcomposer, tgrouping, songYear, tlocation, trackNumber, tgenre, bitRate, sampleRate, songTime, albumId, artistId);
//		cout << "musicDB: sprintf(SQLStmt: "  << SQLStmt << endl;
	}
	else
	{
		sprintf(SQLStmt, "INSERT into Music.presonglibrary (Name ,Artist, AlbumArtists ,Album, Composer, Grouping, SongYear, Location, TrackNumber, Genre, BitRate, SampleRate, SongTime, DateModified, DateAdded, AlbumId, ArtistId, DiscNumber, DiscCount) values ('%s','%s','%s', '%s','%s', '%s' ,%d,'%s',%d, '%s' , %d, %d, %d, ?, ?,  %d, %d, %d, %d)", tname, tartist, talbumArtists, talbum, tcomposer, tgrouping, songYear, tlocation, trackNumber, tgenre, bitRate, sampleRate, songTime, albumId, artistId, diskNumber, diskCount);
//		cout << "musicDB: sprintf(SQLStmt: "  << SQLStmt << endl;
	}
//	cout << "musicDB: sprintf(SQLStmt: "  << SQLStmt << endl;
//Get the current time for date added
	MYSQL_TIME dAdded;
	MYSQL_TIME dModified;
	struct tm *ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);
	/* supply the data to be sent in the ts structure */
	dAdded.year= 1900 + ptr->tm_year;
	dAdded.month= 1 + ptr->tm_mon;
	dAdded.day= ptr->tm_mday;
	dAdded.hour= ptr->tm_hour;
	dAdded.minute= ptr->tm_min;
	dAdded.second= ptr->tm_sec;
	dAdded.second_part = 0;
	dAdded.neg = 0;
	//Get the file modified time for date modified
	struct stat finfo;
	stat (location, &finfo);
	ptr = localtime(&finfo.st_mtime);
	dModified.year= 1900 + ptr->tm_year;
	dModified.month= 1 + ptr->tm_mon;
	dModified.day= ptr->tm_mday;
	dModified.hour= ptr->tm_hour;
	dModified.minute= ptr->tm_min;
	dModified.second= ptr->tm_sec;
	dModified.second_part = 0;
	dModified.neg = 0;
	return (addSongWithTimes(SQLStmt, &dModified, &dAdded));
}

long  musicDB::addSongWithTimes( char * SQLStmt, MYSQL_TIME* dateModified, MYSQL_TIME* dateAdded)
{

	MYSQL_BIND  bind[3];
	MYSQL_STMT  *stmt;
//	cout << "musicDB::addSongWithTimes"  << endl;
	stmt = mysql_stmt_init(&dbaseConnection);
	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		return(0);
	}
//	cout << "musicDB::addSongWithTimes mysql_stmt_prepare" << SQLStmt << endl;
	if (mysql_stmt_prepare( stmt, SQLStmt, strlen(SQLStmt)))
	{
		fprintf(stderr, "\n mysql_stmt_prepare(), INSERT failed");
		fprintf(stderr, " %s: %s\n", location, mysql_stmt_error(stmt));
		return(0);
	}

	memset(bind, 0, sizeof(bind));
	/* set up input buffers for all 3 parameters */
	bind[0].buffer_type= MYSQL_TYPE_TIMESTAMP;
	bind[0].buffer= (char *)dateModified;
	bind[0].is_null= 0;
	bind[0].length= 0;


	bind[1].buffer_type= MYSQL_TYPE_TIMESTAMP;
	bind[1].buffer= (char *)dateAdded;
	bind[1].is_null= 0;
	bind[1].length= 0;

	mysql_stmt_bind_param(stmt, bind);

//	cout << "musicDB::addSongWithTimes mysql_stmt_execute" << stmt << endl;
	/* Execute the INSERT statement - 1*/
	if (mysql_stmt_execute(stmt))
	{
//		cout << "musicDB::addSongWithTimes error 1"  << endl;
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s: %s\n", location, mysql_stmt_error(stmt));
		return(0);
	}

	/* Close the statement */
	if (mysql_stmt_close(stmt))
	{
//		cout << "musicDB::addSongWithTimes eooro 2"  << endl;
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s: %s\n", location, mysql_stmt_error(stmt));
		return(0);
	}
//	cout << "musicDB::addSongWithTimes reutnred success"  << endl;
	return (1);
}

long musicDB::CommitSongsToLibrary()
{
	char SQLStmt[1000];
	MYSQL_ROW row;
	int nrows;
//	cout << "musicDB::CommitSongsToLibrary"  << endl;
	sprintf(SQLStmt, "INSERT into Music.songlibrary (Name ,Artist, Composer, AlbumArtists ,Album, Grouping, SongYear, Location, TrackNumber, Genre,  SongTime, BitRate, SampleRate, DateModified, DateAdded, AlbumId, ArtistId, DiscNumber, DiscCount) select Name ,Artist ,Composer, AlbumArtists ,Album, Grouping, SongYear, Location, TrackNumber, Genre,  SongTime, BitRate, SampleRate, DateModified, DateAdded, AlbumId, ArtistId, DiscNumber, DiscCount from Music.presonglibrary");
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " CommitSongsToLibrary\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}
	return(1); //if successful, return a 1
}

long musicDB::RemoveSongsFromPreSongLibrary()
{
	char SQLStmt[1000];
	MYSQL_ROW row;
	int nrows;
//	cout << "musicDB::RemoveSongsFromPreSongLibrary"  << endl;
	sprintf(SQLStmt, "Delete from Music.presonglibrary");
	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " getArtistID\n");
		fprintf(stderr, " %s: %s\n", location, mysql_error(&dbaseConnection));
		return(0);
	}
	return(1); //if successful, return a 1
}



MYSQL_RES * queryPlayQ()
{
	char queryStmt[5000];
	string message;
	MYSQL_RES *queryPlayQResult;

	sprintf(queryStmt,"SELECT playq.id, playq.songID, Name, Artist, Location, playq.Status, Album, Genre, TrackNumber, SongYear  FROM songlibrary inner join playq on songlibrary.SongIndex = playq.songID where (playq.Status = 'In Queue') order by playq.Arrangement asc, playq.Status, playq.RequestType desc, playq.id asc");
	if(mysql_real_query(&dbaseConnection,queryStmt,strlen(queryStmt)))
	{
		message = "MusicDB.cpp ";
		message.append(__func__);
		message.append(": SQL error");
		myLog.print(logError, message);
		return(0);
//		exit(1);
	}
	queryPlayQResult = mysql_store_result(&dbaseConnection);
	return(queryPlayQResult);
}

struct playQRecord getNextPlayQRecord()
{
	int	i;
	MYSQL_ROW row;
	struct playQRecord pQR;
	MYSQL_RES *queryPlayQResult;

	queryPlayQResult = queryPlayQ();
	row = mysql_fetch_row(queryPlayQResult);
	pQR.id = atol(row[0]);
	pQR.songID = atol(row[1]);
	strcpy(pQR.name,row[2]);
	strcpy(pQR.artist,row[3]);
	strcpy(pQR.location,row[4]);
	strcpy(pQR.status,row[5]);
	strcpy(pQR.album,row[6]);
	strcpy(pQR.genre,row[7]);
	pQR.tracknumber =  atoi(row[8]);
	pQR.songyear = atoi(row[9]);
	mysql_free_result(queryPlayQResult);
	return (pQR);
}


struct playQRecord getCurrentSongInPlayQ()
{
//	char * SQLStmt = "Select id from playq where Status = 'Currently Playing'";
	char * SQLStmt = "SELECT playq.id, playq.songID, Name, Artist, Location, playq.Status, Album, Genre, TrackNumber, SongYear  FROM songlibrary inner join playq on songlibrary.SongIndex = playq.songID where Status = 'Currently Playing'";

	MYSQL_ROW row;
	MYSQL_RES *queryResult;
	int	nrows;
	long returnValue;
	struct playQRecord pQR = {0};
	string message;

	if (mysql_query(&dbaseConnection, SQLStmt))
	{

		message = "MusicDB.cpp ";
		message.append(__func__);
		message.append(": ");
		message.append(mysql_error(&dbaseConnection));
		myLog.print(logError, message);
		return(pQR);
//		exit(0);
	}
	queryResult = mysql_store_result(&dbaseConnection);
	nrows = mysql_num_rows(queryResult);
	if (nrows == 0)
	{
		pQR.id = 0; //if id = 0 then no rows were returned
		message = "MusicDB.cpp "; // Jon
		message.append(__func__);
		message.append(": pQR.id = 0");
		myLog.print(logDebug, message);
	}
	else
	{
		row = mysql_fetch_row(queryResult);
		pQR.id = atol(row[0]);
		pQR.songID = atol(row[1]);
		strcpy(pQR.name,row[2]);
		strcpy(pQR.artist,row[3]);
		strcpy(pQR.location,row[4]);
		strcpy(pQR.status,row[5]);
		strcpy(pQR.album,row[6]);
		strcpy(pQR.genre,row[7]);
		pQR.tracknumber =  atoi(row[8]);
		pQR.songyear = atoi(row[9]);

		message = "MusicDB.cpp "; // Jon
		message.append(__func__);
		message.append(": song returned");
		myLog.print(logDebug, message);

	}
	mysql_free_result(queryResult);
	return (pQR);
}
