#ifndef _STREAMINGNOLOGY_CORE_SNYGPACMP4_H_
#define _STREAMINGNOLOGY_CORE_SNYGPACMP4_H_
#include <string>
#include <map>
#include "SnyIDemuxer.h"
#include "SnyGPACMP4Track.h"

#include <gpac/isomedia.h>
#include <gpac/setup.h>


class SNYGPACMP4: public SNYIDemuxer {
public:
  SNYGPACMP4();
  virtual ~SNYGPACMP4();
public:
  virtual void        setUri(std::string uri);
  virtual SNY_Result  open();
  virtual GPACMediaInfo getMediaInfo();
  //virtual std::string getMediaInfo();
  virtual SNY_Result  selectTrack(int trackNumber);
  virtual SNY_Result  seek(SNY_UI64 position_us);
  virtual SNY_Result  seek(int trackNumber, SNY_UI64 position_us);
  virtual SNY_Bool    reachEndOfFile();
  virtual SNY_Bool    reachEndOfTrack(int trackNumber);
  virtual SnySample*     readSample(int trackNumber);
private:
  std::string mURI;
  GF_ISOFile *mGFISOFile;
  std::map<int, SNYGPACMP4Track*> mSelectedTracks;
};

#endif /* _STREAMINGNOLOGY_CORE_SNYGPACMP4_H_ */
