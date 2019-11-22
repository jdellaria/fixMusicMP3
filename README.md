# prepareMusic
prepareMusic -> to be used instead of addMusic

Supported filetypes:
.mp3 which is played by mpg123.  Only mp3 type streams can be played

Required Libraries -> taglib

install

sudo apt-get install libtag1-dev

sudo apt-get install libmysql++-dev

sudo apt install ffmpeg // for extracting cover image


prepareMusic

 -f -> Force Artist and Album Name of the MP3 files in the MP3StagingDirectory
 
 -y #### -> Force Year of the MP3 files in the MP3StagingDirectory
 
 -l -> Load albums from the staging directory into MP3MusicLibraryDirectory
 
 -c -> Check for errors in the MP3StagingDirectory
 
 no switches simply places the file from MP3OriginalDirectory into MP3StagingDirectory creates the filename and folder structure
 
	
