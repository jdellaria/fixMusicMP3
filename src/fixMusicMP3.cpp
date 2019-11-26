//============================================================================
// Name        : fixMusicMP3.cpp
// Author      : Jon Dellaria
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <DLog.h>
#include <Directory.h>
#include <File.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>


#include <algorithm>
#include <vector>
#include <iterator>


#include "audioTags.h"
#include "configurationFile.h"
#include "musicDB.h"
DLog myLog;

using namespace std;
void doRecurseDirectory(char const * text, int x);

int newAlbum = 1;
long lAlbum = 0;
long lArtist = 0;
int gotCoverJPG = 0;
string destinationDir;
string jpgThumbName = "none";
string coverName = "none";
long myAlbumYear = 0;//Add Music to Library
configurationFile myConfig;

typedef enum duplicateActions {
	ACTION_STAGE = 0,
	ACTION_FORCE,
	ACTION_YEAR,
	ACTION_LOAD,
	ACTION_CHECK
} Action;



musicDB myDB;

int isAppleDropping(char* const fileName);
int isMP3(char* const fileName);
int isJPG(char* const fileName);
int addSongToDB(char* const fileName, char* const thumbName, char* const grouping);
int configApp();


vector <string> TokenizeString(string line, char tokenSeparator);
int StringToInt (std::string line);

void deleteDirectoryDo(char const* directoyEntry, int directoyEntryType);
void doCreatFileNameFolderStructure(char const* directoyEntry, int directoyEntryType);
void doForceArtistAlbumName(char const* directoyEntry, int directoyEntryType);
void doForceYear(char const * directoyEntry, int directoyEntryType);
void doLoadAlbumsToDatabase(char const * directoyEntry, int directoyEntryType);
void doCheckForTagErrors(char const * directoyEntry, int directoyEntryType);
void removeForbiddenFileNameChar(string* s);
int checkForTagErrors(audioTags *myTags, string sourceFile);

#ifdef JON
#include <string>
#include <codecvt>
#include <locale>

string utf8_to_string(const char *utf8str, const locale& loc)
{
    // UTF-8 to wstring
    wstring_convert<codecvt_utf8<wchar_t>> wconv;
    wstring wstr = wconv.from_bytes(utf8str);
    // wstring to string
    vector<char> buf(wstr.size());
    use_facet<ctype<wchar_t>>(loc).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
    return string(buf.data(), buf.size());
}

int mainqwe(int argc, char* argv[])
{
    string ansi = "Ali Farka Touré & Toumani Diabeté";
    char utf8txt[] = {(char)0xc3,(char) 0xa1, (char)0};

    // I guess you want to use Windows-1252 encoding...
//    ansi = utf8_to_string(utf8txt, locale(".1252"));
	cout << "ansi: " << ansi << endl;
	cout << "ansi: " << ansi << endl;
    // Now do something with the string
    return 0;
}
#endif
//prepareMusic
//-f -> Force Artist and Album Name of the MP3 files in the MP3StagingDirectory
//-y -> Force Year of the MP3 files in the MP3StagingDirectory
//-l -> Load albums from the staging directory into MP3MusicLibraryDirectory
//-c -> Check for errors in the MP3StagingDirectory
//no switches simply places the file from MP3OriginalDirectory into MP3StagingDirectory creates the filename and folder structure

//doForceYear
//doForceArtistAlbumName
//doLoadAlbumsIntoSystem

bool cmdOptionExists(char** begin, char** end, const string& option)
{
    return std::find(begin, end, option) != end;
}

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}
int main(int argc, char *argv[])
{
	int x = 0;
	cout << "hello world" << endl;
	string message;
	audioTags myTags;
	Directory myDirectory;
	File myFile;
	string fileName;
	string destinationFileName = "none";
	size_t found;
	string sourceFile;
	string tempString = "/RAID/Music/Album Music/Edith Piaf/The Best Of/Edith Piaf - Hymne À L'Amour.mp3" ;

//	myDB.queryAlbumSongs();
//	myDB.getNextSongRecord();
//	myTags.get((char*)myDB.location);
//	checkForTagErrors(&myTags,(char*)myDB.location );
//	if (myFile.exist(myDB.location) == 1)
//	sourceFile = myDB.location;
//	std::cout  << endl << "Length of myDB.location: " << sourceFile.length() << endl;
//	std::cout  << endl << "Length of tempString: " << tempString.length() << endl;

		int length = sourceFile.length();
	//	for(int i = 0;i < where.length();i++)
		int i = 0;

		for(i = 0;i < length;i++)
		{
			std::cout  <<  "tempString: " <<  (unsigned int ) tempString[i]   <<  " myDB.location: "   <<  (unsigned int) myDB.location[i] << endl;

		}



		if (myFile.exist(tempString) == 1)
		{
			std::cout  << endl << "tempString: File Exists: "  << endl;
		}
		else
		{
			std::cout  << endl << "tempString: File DOES NOT Exists: "  << endl;
		}

		if (myFile.exist(sourceFile) == 1)
		{
			std::cout  << endl << "sourceFile: File Exists: "  << endl;
		}
		else
		{
			std::cout  << endl << "sourceFile: File DOES NOT Exists: "  << endl;
		}
	std::cout  << endl << "Comments: "  << endl;
	std::cout << "SongIndex: " << myDB.songID << " - Comments: " << myTags.comments << endl;


	std::cout  << endl << "Database: "  << endl;
	std::cout << "Album: " << myDB.album << " - Song: " << myDB.name << " - Track: " << myDB.trackNumber << " - Location: " << myDB.location  << endl;
	std::cout  << endl << "MP3File: "  << endl;
	std::cout << "Album: " << myTags.album.ascii << " - Song: " << myTags.title.ascii << " - Track: " << myTags.track << " - Location: "  << myTags.location.ascii << endl;
#ifdef JON



	destinationDir = "/RAID/Music/Album Music/";



	myDirectory.Recurse("/RAID/Music/Album Music/Edith Piaf/The Best Of/", doLoadAlbumsToDatabase);

	myTags.get((char*)tempString.c_str());
	myDB.setAlbum(myTags.album.ascii);
	myDB.setArtist(myTags.artist.ascii);
	myDB.setComposer(myTags.composers.ascii);
	myDB.setAlbumArtists(myTags.albumArtists.ascii);
	myDB.setSongName(myTags.title.ascii);
	myDB.setGenre(myTags.genre.ascii);
	myDB.setTrackNumber(myTags.track);
	myDB.setSongYear(myTags.year);
	myDB.setBitRate(myTags.bitrate);
	myDB.setSampleRate(myTags.sampleRate);
	myDB.setSongTime(myTags.length);
	myDB.setDiskNumber(myTags.disk);
	myDB.setDiskCount(myTags.diskcount);
//				cout << "myDB.diskCount: " << myDB.diskCount  << endl;
//				cout << "myDB.diskNumber: " << myDB.diskNumber  << endl;
	myDB.setLocation ((char*)tempString.c_str());
	myDB.setGrouping("Album");

	myDB.addSongToPreSongLibrary();

	myDB.CommitSongsToLibrary();

#endif


//#ifdef JON
//	myDB.comments = myTags.comments;
//	myDB.updateSongComments();

	cout << " looking into DB" << endl;
x = 0;
	myDB.queryAlbumSongs();
	while (myDB.getNextSongRecord() == 1)
	{
		x++;
		if (myTags.get((char*)myDB.location) == 1)
		{
			checkForTagErrors(&myTags,(char*)myDB.location );
			myDB.comments = myTags.comments;
			myDB.updateSongComments();
			myTags.set((char*)myDB.location);
			std::cout << "Album: " << myDB.album << " - Song: " << myDB.name << " - Track: " << myDB.trackNumber << " - Location: " << myDB.location  <<endl;
		}
		else
		{
			myDB.comments = "No File Found";
			myDB.updateSongComments();
		}
	}
//#endif
	myDB.mysqlFree();
	cout << x << " Records Found" << endl;

}


int checkForTagErrors(audioTags *mp3Tags, string mp3File)
{
	string message;
	string commentMessage;
	int returnValue = 0;
	vector <string> myTokens;
	int myInt;
	size_t found;
	string fileName;
	string tempString;
	string AlbumName;
	string ArtistName;
	int errors = 0;



	found = mp3File.find_last_of("/\\");
	fileName = mp3File.substr(found+1);
	tempString = mp3File.substr(0,found);

	found = tempString.find_last_of("/\\");
	AlbumName = tempString.substr(found+1);
	tempString = tempString.substr(0,found);

	found = tempString.find_last_of("/\\");
	ArtistName = tempString.substr(found+1);
	tempString = tempString.substr(0,found);

	message = "checkForTagErrors: fileName: ";
	message.append(fileName);
	message.append(" AlbumName: ");
	message.append(AlbumName);
	message.append(" ArtistName: ");
	message.append(ArtistName);
	myLog.print(logDebug, message);

	if (mp3Tags->track == 0)
	{
		commentMessage.append("No Track: ");
		mp3Tags->track = myDB.trackNumber;
		returnValue = 1;
		message = "No Track Information ";
		message.append(mp3File);
		myLog.print(logError, message);
	}
	if (mp3Tags->year == 0)
	{
		mp3Tags->year = myDB.songYear;
		returnValue = 1;
		commentMessage.append("No Year: ");
		message = "No Year Information ";
		message.append(mp3File);
		myLog.print(logError, message);
	}
	if (mp3Tags->title.utf8.length() == 0)
	{
		mp3Tags->title.ascii = myDB.name;
		returnValue = 1;
		commentMessage.append("No Title: ");
		message = "No Title Information ";
		message.append(mp3File);
		myLog.print(logError, message);
	}
	if (mp3Tags->album.utf8.length() == 0)
	{
		mp3Tags->album.ascii = myDB.album;
		returnValue = 1;
		commentMessage.append("No Album: ");
		message = "No Album Information. Setting Album name to its Folder Name: ";
		message.append("'");
		message.append(AlbumName);
		message.append("' ");
		message.append(mp3File);
		myLog.print(logError, message);
	}
	if (mp3Tags->artist.utf8.length() == 0)
	{
		mp3Tags->artist.ascii = myDB.artist;
		returnValue = 1;
		commentMessage.append("No Artist: ");

		message = "No Artist Information. Setting Artist name to its Folder Name: ";
		message.append("'");
		message.append(ArtistName);
		message.append("' ");
		message.append(mp3File);
		myLog.print(logError, message);
	}
	if (returnValue == 0)
	{
		commentMessage = "No Errors:";
	}


	mp3Tags->comments = commentMessage;
	return(returnValue);
}

int main1(int argc, char *argv[])
{
	Directory myDirectory;
	audioTags myTags;
	Action myAction = ACTION_STAGE;

	string message;
	char intbuffer[20];

	cout << "Add Music to Library" << endl;

	myConfig.getConfiguration("./config.xml");
	configApp();


	std::cout << "myConfig.MP3OriginalDirectory - " << myConfig.MP3OriginalDirectory << std::endl;
	std::cout << "myConfig.MP3StagingDirectory - " << myConfig.MP3StagingDirectory << std::endl;
	std::cout << "myConfig.MP3MusicLibraryDirectory - " << myConfig.MP3MusicLibraryDirectory << std::endl;

	destinationDir = myConfig.MP3StagingDirectory;

	// Set the action defaults
	    if(cmdOptionExists(argv, argv+argc, "-force")||cmdOptionExists(argv, argv+argc, "-f"))
	    {
	    	myAction = ACTION_FORCE;
	    }
	    if(cmdOptionExists(argv, argv+argc, "-load")||cmdOptionExists(argv, argv+argc, "-l"))
	    {
	    	myAction = ACTION_LOAD;
	    }
	    if(cmdOptionExists(argv, argv+argc, "-check")||cmdOptionExists(argv, argv+argc, "-c"))
	    {
	    	myAction = ACTION_CHECK;
	    }

	    char * STRForcedYear = getCmdOption(argv, argv + argc, "-y"); // get the Year and then force the year
	    if (STRForcedYear)
	    {
	    	char *pNext;
	    	myAlbumYear = strtol (STRForcedYear, &pNext, 10);
	    	myAction = ACTION_YEAR;
	    }

//		destinationDir = myConfig.MP3MusicLibraryDirectory;
//	    doLoadAlbumsToDatabase("/home/jdellaria/Desktop/New Albums/Staging/Guns n Roses/Night of the Livid Redhead/1-01 Intro.mp3", DIRECTORYENTRYTYPE_REG);
/*
	    myTags.get("/home/jdellaria/Desktop/Album Music/Original/Ali Farka Touré & Toumani Diabeté/In The Heart Of The Moon/01 Debe.mp3");
	    std::cout << "myTags.track: " << myTags.track << '\n';
	    std::cout << "myTags.artist: " << myTags.artist << '\n';
	    std::cout << "myTags.album: " << myTags.album << '\n';
*/
//	    checkForTagErrors(myTags, "/home/jdellaria/Desktop/New Albums/Original/Jeff Beck - Discography (1965-2009) 320kbps/Jeff Beck Group/1967-1971 - BBC Radio 1 Sessions/09 Got The Feeling.mp3");


	    switch(myAction)
	    {
    		case ACTION_STAGE:
 //   			std::cout << "ACTION_STAGE" << '\n';
    			message = "ACTION_STAGE: Staging files to ";
    			message.append(myConfig.MP3OriginalDirectory);
    			myLog.print(logInformation, message);
    			myDirectory.Recurse(myConfig.MP3OriginalDirectory.c_str(), doCreatFileNameFolderStructure);
	    	    if(myLog.numberOfErrors == 0)
	    		{
	    			message = "ACTION_STAGE: No Errors in original directory: ";
	    			message.append(myConfig.MP3OriginalDirectory);
	    			myLog.print(logInformation, message);
	    		}

    		break;

	    	case ACTION_FORCE:
//	    		std::cout << "ACTION_FORCE" << '\n';
    			message = "ACTION_FORCE: Forcing Album and Artist Name in folder ";
    			message.append(myConfig.MP3StagingDirectory);
    			myLog.print(logInformation, message);
	    		myDirectory.Recurse(myConfig.MP3StagingDirectory.c_str(), doForceArtistAlbumName);
	    	    if(myLog.numberOfErrors == 0)
	    		{
	    			message = "ACTION_FORCE: No Errors in staging directory: ";
	    			message.append(myConfig.MP3StagingDirectory.c_str());
	    			myLog.print(logInformation, message);
	    		}

	    		break;
	    	case ACTION_YEAR:
//	    		std::cout << "ACTION_YEAR" << '\n';
    			message = "ACTION_YEAR: Forcing year ";
    			message.append(STRForcedYear);
    			message.append(" in folder ");
    			message.append(myConfig.MP3StagingDirectory);
    			myLog.print(logInformation, message);
	    		myDirectory.Recurse(myConfig.MP3StagingDirectory.c_str(), doForceYear);
	    		break;

	    	case ACTION_CHECK:
//	    		std::cout << "ACTION_YEAR" << '\n';
    			message = "ACTION_CHECK Checking for Errors in directory: ";
    			message.append(myConfig.MP3StagingDirectory);
    			myLog.print(logInformation, message);
	    		myDirectory.Recurse(myConfig.MP3StagingDirectory.c_str(), doCheckForTagErrors);

	    	    if(myLog.numberOfErrors == 0)
	    		{
	    			message = "ACTION_CHECK: No Errors in staging directory: ";
	    			message.append(myConfig.MP3StagingDirectory.c_str());
	    			myLog.print(logInformation, message);
	    		}

	    		break;

	    	case ACTION_LOAD:
    			message = "ACTION_LOAD: Loading Albums from ";
    			message.append(myConfig.MP3StagingDirectory);
    			message.append(" into Database ");
    			myLog.print(logInformation, message);

	    		destinationDir = myConfig.MP3MusicLibraryDirectory;
	    		myDirectory.Recurse(myConfig.MP3StagingDirectory.c_str(), doLoadAlbumsToDatabase);


	    		if(myLog.numberOfErrors == 0)
	    		{
	    			myDB.CommitSongsToLibrary();

	    			myDB.RemoveSongsFromPreSongLibrary();
	    			message = "ACTION_LOAD: No Errors. Removing files and directories in: ";
	    			message.append(destinationDir);
	    			myLog.print(logInformation, message);
	    			myDirectory.Recurse(myConfig.MP3StagingDirectory.c_str(), deleteDirectoryDo);
	    		}

	    		break;
	    }

		message = "There were ";
		sprintf(intbuffer,"%d", myLog.numberOfErrors);
		message.append(intbuffer);
		message.append(" errors found.");
		myLog.print(logWarning, message);

}


int mainRemoveForbiddenCharacters()
{
    std::string str = "\"Book\":";
    std::cout << str << std::endl;
    removeForbiddenFileNameChar(&str);
    std::cout << str << std::endl;
    return 0;
}


static char ClearForbidden(char toCheck)
{
	const std::string forbiddenChars = "\\/:?\"<>|";

	const std::string allowedChars = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ()[]_-.";

    if(allowedChars.find(toCheck) == string::npos)
    {
         return '_';
    }

/*    if(forbiddenChars.find(toCheck) != string::npos)
    {
         return '_';
    }
*/
    return toCheck;
}

void removeForbiddenFileNameChar(string* s)
{
    string::iterator it;

    std::transform(s->begin(), s->end(), s->begin(), ClearForbidden);
/*    string illegalChars = "\\/:?\"<>|";
    for (it = s->begin() ; it < s->end() ; ++it){
        bool found = illegalChars.find(*it) != string::npos;
        if(found){
            *it = '_';
        }
    }*/

}

int mainIsDigitTest()
{
	string myLine = "Browned Eyed Girl (Live).mp3";
	vector <string> myTokens;
	int myInt;

	myTokens = TokenizeString(myLine, ' ');
    // Printing the token vector
    for(int i = 0; i < myTokens.size(); i++)
        cout << myTokens[i] << '\n';
    myInt = StringToInt(myTokens[0]);
    cout << "myInt " << myInt << '\n';
}

int StringToInt (std::string line)
{
	  std::string::size_type sz;   // alias of size_t
	  int i_dec = -1;
	  if (isdigit(line[0]))
	  {
		  i_dec = std::stoi (line,&sz);
//		  std::cout << line << ": " << i_dec << " and [" << line.substr(sz) << "]\n";
	  }

	  return i_dec;
}

vector <string> TokenizeString(string line, char tokenSeparator)
{
    // Vector of string to save tokens
    vector <string> tokens;

    // stringstream class check1
    stringstream check1(line);

    string intermediate;

    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, tokenSeparator))
    {
        tokens.push_back(intermediate);
    }

    return tokens;
}


void deleteDirectoryDo(char const* directoyEntry, int directoyEntryType)
{
	Directory myDirectory;
	string message;

	if (directoyEntryType == DIRECTORYENTRYTYPE_DIR)
	{
		message = "Deleting directory ";
		message.append(directoyEntry);
		myLog.print(logWarning, message);
		myDirectory.Remove(directoyEntry);
	}
}



void doCheckForTagErrors(char const * directoyEntry, int directoyEntryType)
//doCheckForTagErrors
//this Checks For Tag Errors of all the MP3 files in the folder

{
	string message;
	audioTags myTags;

	File myFile;
	string fileName;
	string destinationFileName = "none";
	size_t found;
	string sourceFile;
	string tempString;


	sourceFile = directoyEntry;

	if (directoyEntryType == DIRECTORYENTRYTYPE_DIR)
	{
		newAlbum = 1; // New directory means new Album
		gotCoverJPG = 0;
//		cout << "recurseDo: directoyEntryType == DIRECTORYENTRYTYPE_DIR"  << endl;
//		printf("New Album!!! %s\n", directoyEntry);
	}

	if(directoyEntryType == DIRECTORYENTRYTYPE_REG) // if it is a file and a duplicate... then remove
	{


		message.clear();
		message.append(directoyEntry);
		myLog.print(logDebug, message);


		if (!isAppleDropping((char* const)fileName.c_str())) // Apple leaves files that begin with ._ and we need to forget about them.
		{

			if (isMP3((char* const)sourceFile.c_str()))
			{

				myTags.get((char*)sourceFile.c_str());
				checkForTagErrors(&myTags,(char*)sourceFile.c_str() );

			}
		}

	}
}


void doForceYear(char const * directoyEntry, int directoyEntryType)
//doForceYear
//this forces the year of all the MP3 files in the folder to be the year specified

{
	string message;
	audioTags myTags;

	File myFile;
	string fileName;
	string destinationFileName = "none";
	size_t found;
	string sourceFile;
	string tempString;


	sourceFile = directoyEntry;

	if (directoyEntryType == DIRECTORYENTRYTYPE_DIR)
	{
		newAlbum = 1; // New directory means new Album
		gotCoverJPG = 0;
//		cout << "recurseDo: directoyEntryType == DIRECTORYENTRYTYPE_DIR"  << endl;
//		printf("New Album!!! %s\n", directoyEntry);
	}

	if(directoyEntryType == DIRECTORYENTRYTYPE_REG) // if it is a file and a duplicate... then remove
	{


		message.clear();
		message.append(directoyEntry);
		myLog.print(logDebug, message);


		if (!isAppleDropping((char* const)fileName.c_str())) // Apple leaves files that begin with ._ and we need to forget about them.
		{

			if (isMP3((char* const)sourceFile.c_str()))
			{

				myTags.get((char*)sourceFile.c_str());
				myTags.year = myAlbumYear;
				myTags.set((char*)sourceFile.c_str());
			}
		}

	}
}

void doForceArtistAlbumName(char const * directoyEntry, int directoyEntryType)
//doForceArtistAlbumName
//this takes the folder structure and forces the Artist name and Album names to be to folder structure names.

{
	string message;
	audioTags myTags;

	File myFile;
	string fileName;
	string destinationFileName = "none";
	size_t found;
	string sourceFile;
	string tempString;
	string AlbumName;
	string ArtistName;

	sourceFile = directoyEntry;

	if (directoyEntryType == DIRECTORYENTRYTYPE_DIR)
	{
		newAlbum = 1; // New directory means new Album
		gotCoverJPG = 0;
//		cout << "recurseDo: directoyEntryType == DIRECTORYENTRYTYPE_DIR"  << endl;
//		printf("New Album!!! %s\n", directoyEntry);
	}

	if(directoyEntryType == DIRECTORYENTRYTYPE_REG) // if it is a file and a duplicate... then remove
	{


		AlbumName = "UnknownAlbum";
		ArtistName = "UnknownArtist";

		message.clear();
		message.append(directoyEntry);
		myLog.print(logDebug, message);


		if (!isAppleDropping((char* const)fileName.c_str())) // Apple leaves files that begin with ._ and we need to forget about them.
		{

			if (isMP3((char* const)sourceFile.c_str()))
			{

				found = sourceFile.find_last_of("/\\");
				fileName = sourceFile.substr(found+1);
				tempString = sourceFile.substr(0,found);

				found = tempString.find_last_of("/\\");
				AlbumName = tempString.substr(found+1);
				tempString = tempString.substr(0,found);

				found = tempString.find_last_of("/\\");
				ArtistName = tempString.substr(found+1);
				tempString = tempString.substr(0,found);
				cout << "AlbumName: " << AlbumName << endl;
				cout << "ArtistName: " << ArtistName << endl;

				myTags.get((char*)sourceFile.c_str());
				myTags.album.utf8 = AlbumName;
				myTags.artist.utf8 = ArtistName;
				myTags.set((char*)sourceFile.c_str());
			}
		}

	}

}


void doCreatFileNameFolderStructure(char const * directoyEntry, int directoyEntryType)
//doCreatFileNameFolderStructure
//this standardized the filename and folder structure for music files.
//file name is this format "Disk-tracknumber songname.mp3"
//this is the same format that iTunes uses
//Creates folder structure ArtistName/AlbumName
{
	string message;
	audioTags myTags;

	File myFile;
	string fileName;
	string destinationFileName = "none";
	size_t found;
	string sourceFile;
	string tempString;
	string AlbumName;
	string ArtistName;

	char ibuffer [33];



	sourceFile = directoyEntry;

	if (directoyEntryType == DIRECTORYENTRYTYPE_DIR)
	{
		newAlbum = 1; // New directory means new Album
		gotCoverJPG = 0;
//		cout << "recurseDo: directoyEntryType == DIRECTORYENTRYTYPE_DIR"  << endl;
//		printf("New Album!!! %s\n", directoyEntry);
	}

	if(directoyEntryType == DIRECTORYENTRYTYPE_REG) // if it is a file and a duplicate... then remove
	{

		int returnValue = 0;
		AlbumName = "UnknownAlbum";
		ArtistName = "UnknownArtist";

		message.clear();
		message.append(directoyEntry);
		myLog.print(logDebug, message);

		found = sourceFile.find_last_of("/\\");
		fileName = sourceFile.substr(found+1);
		tempString = sourceFile.substr(0,found); // tempString has the directory name

		if (!isAppleDropping((char* const)fileName.c_str())) // Apple leaves files thatnbegin with ._ and we need to forget about them.
		{

			if (isMP3((char* const)sourceFile.c_str()))
			{
				myTags.get((char*)sourceFile.c_str());
				returnValue = checkForTagErrors(&myTags, sourceFile);
				if (returnValue == 0) // if success.. no errors
				{

	//				std::string s = std::to_string(myTags.track);
	//				auto s = std::to_string(42);

					destinationFileName = tempString + "/";
					if (myTags.disk > 0)
					{
						sprintf(ibuffer, "%d",  myTags.disk);
	//					destinationFileName.append("/");
						destinationFileName.append(ibuffer);
						destinationFileName.append("-");
					}

					sprintf(ibuffer, "%d",  myTags.track);

					if (myTags.track < 10)
					{
						destinationFileName.append("0");
	//					destinationFileName = tempString + "/0";
					}
					else
					{
	//					destinationFileName.append("/");
	//					destinationFileName = tempString + "/";
					}
					destinationFileName.append(ibuffer);
					destinationFileName.append(" ");
					fileName = myTags.title.utf8;
					removeForbiddenFileNameChar(&fileName);
					destinationFileName.append( fileName );
					destinationFileName.append(".mp3");

					cout << "destinationFileName: " << destinationFileName << endl;
					myFile.rename(sourceFile.c_str(), destinationFileName.c_str() );

					if (myTags.albumArtists.utf8.length() > 0)
					{
						ArtistName = myTags.albumArtists.utf8;
					}
					else if (myTags.artist.utf8.length() > 0)
					{
						ArtistName = myTags.artist.utf8;
					}
					if (myTags.album.utf8.length() > 0)
					{
						AlbumName = myTags.album.utf8;
					}


					sourceFile = destinationFileName;

					found = sourceFile.find_last_of("/\\");
					fileName = sourceFile.substr(found+1);
					tempString = sourceFile.substr(0,found);

	/*				found = tempString.find_last_of("/\\");
					AlbumName = tempString.substr(found+1);
					tempString = tempString.substr(0,found);

					found = tempString.find_last_of("/\\");
					ArtistName = tempString.substr(found+1);
					tempString = tempString.substr(0,found);
					;*/
					removeForbiddenFileNameChar(&ArtistName);
					removeForbiddenFileNameChar(&AlbumName);
					removeForbiddenFileNameChar(&fileName);

					destinationFileName = destinationDir + ArtistName + "/" + AlbumName + "/" + fileName;

	//				cout << "destinationFileName: " << destinationFileName << endl;
	//				cout << "sourceFile: " << sourceFile << endl;

					returnValue = myFile.copy(sourceFile,destinationFileName);
					if (returnValue == 0) // if success
					{
						string directoryName;
						string albumImageName;
						File myCoverImageFile;
						message = "Copying file:";
						message.append(sourceFile);
						message.append(" to -> ");
						message.append(destinationFileName);
						myLog.print(logInformation, message);
						myFile.remove( sourceFile.c_str()  );
						//No we can creat the cover.jpg if it is not there.
						found = destinationFileName.find_last_of("/\\"); // Get the folder name that the file resides in
						directoryName = destinationFileName.substr(0,found+1);
						albumImageName = directoryName + "cover.jpg";
						if(myCoverImageFile.exist(albumImageName.c_str()) == 0)
						{
							myTags.getImage(destinationFileName.c_str(), albumImageName.c_str());
	//						cout <<"Writing Album cover for "  << albumImageName << endl;
						}
					}
					else
					{
						message = "Error copying file: ";
						message.append(myFile.errorMessageBuffer);
						message.append(": ");
						message.append(sourceFile);
						message.append(" to -> ");
						message.append(destinationFileName);
						myLog.print(logError, message);
					}
				}

			}
		}

	}
}


void doLoadAlbumsToDatabase(char const * directoyEntry, int directoyEntryType)
{
	string message;
	audioTags myTags;

	File myFile;
	string fileName;
	string destinationFileName = "none";
	size_t found;
	string sourceFile;
	string tempString;
	string AlbumName;
	string ArtistName;
	char albumIDString[10];

	int returnValue;

	sourceFile = directoyEntry;

	if (directoyEntryType == DIRECTORYENTRYTYPE_DIR)
	{
		// we need to get more information about this directory... could be additional directories under the album we are loading.. for now... we do not support sub folders under the Album name
		newAlbum = 1; // New directory means new Album
		gotCoverJPG = 0;
//		cout << "recurseDo: directoyEntryType == DIRECTORYENTRYTYPE_DIR"  << endl;
//		printf("New Album!!! %s\n", directoyEntry);
	}

	if(directoyEntryType == DIRECTORYENTRYTYPE_REG) // if it is a file and a duplicate... then remove
	{

		found = sourceFile.find_last_of("/\\");
		fileName = sourceFile.substr(found+1);
		tempString = sourceFile.substr(0,found);

		found = tempString.find_last_of("/\\");
		AlbumName = tempString.substr(found+1);
		tempString = tempString.substr(0,found);

		found = tempString.find_last_of("/\\");
		ArtistName = tempString.substr(found+1);
		tempString = tempString.substr(0,found);
		destinationFileName = destinationDir + ArtistName + "/" + AlbumName + "/" + fileName;

//		cout << "Artist: " << ArtistName << " - Album: " << AlbumName << endl;
//		printf("%s\n%s\n\n", directoyEntry,destinationFileName.c_str());

		message.clear();
		message.append(directoyEntry);
		myLog.print(logWarning, message);


		if (!isAppleDropping((char* const)fileName.c_str())) // Apple leaves files thatnbegin with ._ and we need to forget about them.
		{

			returnValue = myFile.copy(sourceFile,destinationFileName);

			if (returnValue == 0) // if success
			{
				message = "Copying file:";
				message.append(sourceFile);
				message.append(" to -> ");
				message.append(destinationFileName);
				myLog.print(logInformation, message);
			}
			else
			{
				message = "Error copying file: ";
				message.append(myFile.errorMessageBuffer);
				message.append(": ");
				message.append(sourceFile);
				message.append(" to -> ");
				message.append(destinationFileName);
				myLog.print(logError, message);
			}
	// See if this file is the cover file. if it is... then we can copy over the thumb image as well when we have the album ID
			if (strcasecmp (fileName.c_str(),"cover.jpg") == 0)
			{
				gotCoverJPG = 1;
				coverName = sourceFile;
			}
			if (strcasecmp (fileName.c_str(),"cover.jpeg") == 0)
			{
				gotCoverJPG = 1;
				coverName = sourceFile;
			}

			if (isMP3((char* const)sourceFile.c_str()))
			{
				myTags.get((char*)sourceFile.c_str());
				myDB.setAlbum(myTags.album.utf8);
				myDB.setArtist(myTags.artist.utf8);
				myDB.setComposer(myTags.composers.utf8);
				myDB.setAlbumArtists(myTags.albumArtists.utf8);
				myDB.setSongName(myTags.title.utf8);
				myDB.setGenre(myTags.genre.utf8);
				myDB.setTrackNumber(myTags.track);
				myDB.setSongYear(myTags.year);
				myDB.setBitRate(myTags.bitrate);
				myDB.setSampleRate(myTags.sampleRate);
				myDB.setSongTime(myTags.length);
				myDB.setDiskNumber(myTags.disk);
				myDB.setDiskCount(myTags.diskcount);
//				cout << "myDB.diskCount: " << myDB.diskCount  << endl;
//				cout << "myDB.diskNumber: " << myDB.diskNumber  << endl;
				myDB.setLocation ((char*)destinationFileName.c_str());
				myDB.setGrouping("Album");

				if (newAlbum == 1)
				{
					newAlbum = 0;
					lAlbum = myDB.addAlbum();
					sprintf(albumIDString, "%ld", lAlbum);
					jpgThumbName = destinationDir + ArtistName + "/" + AlbumName + "/" + albumIDString + ".w300.thumb";
					myDB.setThumbLocation ((char*)jpgThumbName.c_str());
					myDB.setAlbumId(lAlbum);
					myDB.updateAlbumCover();
					lArtist = myDB.addArtist();
					myDB.setArtistId(lArtist);
		//			cout << "updateAlbumCover: " << jpgThumbName  << endl;
				}


				myDB.setAlbumId(lAlbum);
				myDB.setArtistId(lArtist);
				myDB.addSongToPreSongLibrary();

//				addSongToDB((char* const)destinationFileName.c_str(), (char* const)jpgThumbName.c_str(), (char* const)"Album");

//				cout << "updateAlbumCover: " << jpgThumbName  << endl;

			}
//				printf("thumb %s\n\n", jpgThumbName.c_str());
			if( (gotCoverJPG == 1) && (newAlbum == 0))
			{
				returnValue = myFile.copy(coverName,jpgThumbName);
				gotCoverJPG = 0; // Set got cover back to 0 so that we dont to copy it again

				if (returnValue == 0) // if success
				{
					message = "Copying file:";
					message.append(coverName);
					message.append(" to -> ");
					message.append(jpgThumbName);
					myLog.print(logInformation, message);
				}
				else
				{
					message = "Error copying file: ";
					message.append(myFile.errorMessageBuffer);
					message.append(": ");
					message.append(coverName);
					message.append(" to -> ");
					message.append(jpgThumbName);
					myLog.print(logError, message);
				}
			}

		}
	}
}

#ifdef NOTNEEDED

int addSongToDB(char* const fileName, char* const thumbName, char* const grouping)
{
	audioTags myTags;

	myTags.get(fileName);
	myDB.setAlbum(myTags.album);
	myDB.setArtist(myTags.artist);
	myDB.setSongName(myTags.title);
	myDB.setGenre(myTags.genre);
	myDB.setTrackNumber(myTags.track);
	myDB.setSongYear(myTags.year);
	myDB.setBitRate(myTags.bitrate);
	myDB.setSampleRate(myTags.sampleRate);
	myDB.setSongTime(myTags.length);
	myDB.setLocation (fileName);
	myDB.setGrouping(grouping);

	if (strcmp ("Album", grouping) != 0) // is this is not an album.. set album and artist ID to 0
	{
		lAlbum = 0;
		lArtist = 0;
	}

	myDB.setAlbumId(lAlbum);
	myDB.setArtistId(lArtist);
	myDB.addSongToPreSongLibrary();

	return 0;
}
#endif

int isMP3(char* const fileName)
{
	if (strcasecmp (&fileName[strlen(fileName)-3],"MP3") == 0)
	{
		return (1);
	}
	return(0);
}


int isAppleDropping(char* const fileName)
{
	if (strstr (&fileName[0],"._") != 0) //strstr returns a pointer to the string if it is found. Otherwise a 0 is returned.
	{
		return (1);
	}
	return(0);
}


int isJPG(char* const fileName)
{
	if (strcasecmp (&fileName[strlen(fileName)-3],"JPG") == 0)
	{
		return (1);
	}
	if (strcasecmp (&fileName[strlen(fileName)-4],"JPEG") == 0)
	{
		return (1);
	}
	return(0);
}

int configApp()
{
	string message;

	myLog.logFileName = myConfig.logFileName;
	myLog.printFile = myConfig.logPrintFile;
	myLog.printScreen = myConfig.logPrintScreen;
	myLog.printTime = myConfig.logPrintTime;

	if (myConfig.logValue.find("logDebug")!=string::npos)
	{
		myLog.logValue = logDebug;
		message = "myLog.logValue = logDebug";
		myLog.print(logInformation, message);
	}
	if (myConfig.logValue.find("logInformation")!=string::npos)
	{
		myLog.logValue = logInformation;
		message = "myLog.logValue = logInformation";
		myLog.print(logInformation, message);
	}
	if (myConfig.logValue.find("logWarning")!=string::npos)
	{
		myLog.logValue = logWarning;
		message = "myLog.logValue = logWarning";
		myLog.print(logInformation, message);
	}
	if (myConfig.logValue.find("logError")!=string::npos)
	{
		myLog.logValue = logError;
		message = "myLog.logValue = logError";
		myLog.print(logInformation, message);
	}

	return (1);
}

