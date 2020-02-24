#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNY_VOD_SINGLE_BITRATE_MANIFEST_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNY_VOD_SINGLE_BITRATE_MANIFEST_H_
#include <string>
#include "SnyType.h"
#include "SnyDataBuffer.h"

class SNYVODSingleBitRateManifest{
public:
	SNYVODSingleBitRateManifest();
	~SNYVODSingleBitRateManifest();

public:
  void setChannelID(std::string channelID);
  void setMediaDuration(SNY_SI64 duration);
  std::string getManifestUri();
	SnyDataBuffer getSingleBitRatePlaylist();
private:
  std::string channelID;
  SNY_SI64    duration;
};

#endif //!_STREAMINGNOLOGY_MEDIASERVER_SNY_VOD_SINGLE_BITRATE_MANIFEST_H_
