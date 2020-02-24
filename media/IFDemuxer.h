#ifndef STN_MEDIA_H
#define STN_MEDIA_H

#include <string>

using namespace std;

namespace STREAMINGNOLOGY_MEDIA {

  class IFDemuxer {
    public:
    static IFDemuxer* CreateInstance();

    public:
    virtual void SetUri(string uri) = 0;
    virtual void SelectTrackIndex(int trackIndex) = 0;
    virtual int  Open() = 0;
    virtual bool IsEndOfFile() = 0;
    virtual int  Read() = 0;
  };

}

#endif //!STN_MEDIA_H