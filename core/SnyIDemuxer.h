#ifndef _STREAMINGNOLOGY_CORE_SNYIDEMUXER_H_
#define _STREAMINGNOLOGY_CORE_SNYIDEMUXER_H_
#include <string>
#include "SnySample.h"
#include "SnyType.h"
#include "SnyResults.h"
#include "SnyGPACMediaInfo.h"

class SNYIDemuxer {
public:
  SNYIDemuxer() {}
  virtual ~SNYIDemuxer() {}
  static SNYIDemuxer* createInstance();
public:
  virtual void        setUri(std::string uri) = 0;
  virtual SNY_Result  open() = 0;
  virtual GPACMediaInfo getMediaInfo() = 0;
  //virtual std::string getMediaInfo() = 0;
  virtual SNY_Result  selectTrack(int trackNumber) = 0;
  virtual SNY_Result  seek(SNY_UI64 position_us) = 0;
  virtual SNY_Result  seek(int trackNumber, SNY_UI64 position_us) = 0;
  virtual SNY_Bool    reachEndOfFile() = 0;
  virtual SNY_Bool    reachEndOfTrack(int trackNumber) = 0;
  virtual SnySample*     readSample(int trackNumber) = 0;
};

#endif /* _STREAMINGNOLOGY_CORE_SNYIDEMUXER_H_ */
