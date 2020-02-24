#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "SnyConstants.h"
#include "SnyPlaylist.h"

SNYPlaylist::SNYPlaylist(std::string protocol, std::string uri) {
  this->protocol = protocol;
  this->uri = uri;
}

SNYPlaylist::SNYPlaylist(const SNYPlaylist &object) { 
  operator=(object);
}

void SNYPlaylist::operator=(const SNYPlaylist &object) { 
  this->protocol = object.protocol;
  this->uri = object.uri;
}

void SNYPlaylists::push(SNYPlaylist pl) {
  playlists.push_back(pl);
}

SnyDataBuffer SNYPlaylists::toJSON() { 
  SnyDataBuffer playlist_buffer;
  
  if (!playlists.empty()) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    
    writer.StartObject();
    writer.Key("data");
    writer.StartArray();
    for (auto iter = playlists.begin(); iter != playlists.end(); iter++) {
      writer.StartObject();
      writer.Key(SNY_MEDIASERVER_PLAYLIST_PROTOCOL_C.data());
      writer.String((*iter).protocol.data());
      writer.Key(SNY_MEDIASERVER_PLAYLIST_URI_C.data());
      writer.String((*iter).uri.data());
      writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
    
    std::string s = buffer.GetString();
    playlist_buffer.append(s.data(), static_cast<int>(s.size()));
  }
  
  return playlist_buffer;
}




