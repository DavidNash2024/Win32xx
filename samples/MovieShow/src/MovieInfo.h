
#ifndef _MOVIEINFO_H_
#define _MOVIEINFO_H_


struct MovieInfo
{
    MovieInfo()
    {
        ZeroMemory(&lastModifiedTime, sizeof(lastModifiedTime));
        flags = 0;
    }

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
    CString videoType;  // Movie; Live Performance
    CString boxset;
    DWORD   flags;    // for favourites etc.
};

typedef std::list<MovieInfo> MoviesData;

#endif // _MOVIEINFO_H_