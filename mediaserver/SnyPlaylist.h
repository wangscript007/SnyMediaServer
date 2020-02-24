#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNY_MEDIASERVER_TYPE_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNY_MEDIASERVER_TYPE_H_
#include <string>
#include <vector>
#include "SnyType.h"
#include "SnyDataBuffer.h"

class SNYPlaylist {
public:
  SNYPlaylist(std::string protocol, std::string uri);
  ~SNYPlaylist() {}
  
  SNYPlaylist(const SNYPlaylist &object);
  void operator = (const SNYPlaylist &object);
public:
  std::string protocol;
  std::string uri;
};

class SNYPlaylists {
public:
  SNYPlaylists() {}
  ~SNYPlaylists() {}
  void push(SNYPlaylist playlist);
  SnyDataBuffer toJSON();
private:
    std::vector<SNYPlaylist> playlists;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNY_MEDIASERVER_TYPE_H_
