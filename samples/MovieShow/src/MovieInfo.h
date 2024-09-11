
#ifndef _MOVIEINFO_H_
#define _MOVIEINFO_H_


struct MovieInfo
{
    std::vector<BYTE> imageData;
    FILETIME lastModifiedTime;
    CString fileName;
    CString movieName;
    CString duration;
    CString releaseDate;
    CString coverImage;
    CString description;
    CString genre;
    CString actors;
    CString videoType;  // Movie or Live Performance.
    CString boxset;
    DWORD   flags;    // for favourites etc.
};

using MoviesData = std::list<MovieInfo>;

#endif // _MOVIEINFO_H_