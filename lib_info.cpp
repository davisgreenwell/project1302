// g++ -Wall -Wextra -std=c++11 -o lib_info lib_info.cpp
// Davis Greenwell
// Partner: N/A
//^ talked to Dr. Emrich about this on discord more info in write up
// Project 1

/*
==COMMENTS==
1) https://www.codecademy.com/resources/docs/cpp/strings/find

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

//structs from write up
struct Song {
    string title;
    int time;
};

struct Album {
    map<int, Song> songs;
    string name;
    int time = 0;
    int nSongs = 0;
};

struct Artist {
    map<string, Album> albums;
    string name;
    int time = 0;
    int nSongs = 0;
};

//mm:ss to s
//from input into map
int parseTime(const string& time)
{
    size_t colonPos = time.find(':'); //1
    //use size_t because .find returns that type
    int minutes = stoi(time.substr(0, colonPos));
    int seconds = stoi(time.substr(colonPos + 1));
    return minutes * 60 + seconds;
}

//s to mm:ss
//from map into output
string formatTime(int totalSeconds) 
{
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    ostringstream oss; //use oss for formatting
    oss << minutes << ":" << setw(2) << setfill('0') << seconds;
    //makes sure 1m6s is 1:06 instead of 1:6
    return oss.str();
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " file" << endl;
        return 1;
    } //bad user

    ifstream inputFile(argv[1]);
    if (!inputFile) 
    {
        cerr << "Couldn't open file " << argv[1] << endl;
        return 1;
    } //bad file name

    //make the large map
    map<string, Artist> library;
    //input line by line
    string line;

    //while there are lines to get basically just iterating through file
    while (getline(inputFile, line))
    {
        istringstream iss(line); 
        //manipulate the input line so we can assign input to variables
        string title, time, artist, album, genre, trackStr;
        //variables for data to go into

        //makes sure input file is right format/order
        if (!(iss >> title >> time >> artist >> album >> genre >> trackStr)) 
        {
            cerr << "Bad input: " << line << endl;
            continue;
        }

        //convert input format to output format
        replace(title.begin(), title.end(), '_', ' ');
        replace(artist.begin(), artist.end(), '_', ' ');
        replace(album.begin(), album.end(), '_', ' ');

        //change the track into an int
        int track = stoi(trackStr);
        int totalTime = parseTime(time);

        //add the data to the library
        Artist& artistEntry = library[artist];
        artistEntry.name = artist;
        artistEntry.time += totalTime;
        artistEntry.nSongs++;

        //the maps are nested for hierarchy to sort data
        Album& albumEntry = artistEntry.albums[album];
        albumEntry.name = album;
        albumEntry.time += totalTime;
        albumEntry.nSongs++;

        //data is sorted by lib->art->alb->song
        Song songEntry = {title, totalTime};
        albumEntry.songs[track] = songEntry;
    }

    //printing and formatting
    for (const auto& artistPair : library) //iterate through library
    {
        const Artist& artist = artistPair.second; 
        //use pairs for ranged loop artistPair.first would simply be
        //the artists name while second is the artist object
        cout << artist.name << ": " << artist.nSongs << ", " << formatTime(artist.time) << endl;

        for (const auto& albumPair : artist.albums) //through albums
        {
            const Album& album = albumPair.second;
            //same explanation as artist pair
            cout << "        " << album.name << ": " << album.nSongs << ", " << formatTime(album.time) << endl;

            for (const auto& songPair : album.songs) //through songs
            {
                const Song& song = songPair.second;
                //same explanation as artist pair
                cout << "                " << songPair.first << ". " << song.title << ": " << formatTime(song.time) << endl;
            }
        }
    }
    return 0;
}

