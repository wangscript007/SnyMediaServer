#ifndef _STREAMINGNOLOGY_CORE_SNYGPACMP4TRACK_H_
#define _STREAMINGNOLOGY_CORE_SNYGPACMP4TRACK_H_
#include "SnyType.h"
#include "SnyConstants.h"
#include <gpac/mpeg4_odf.h>
#include <gpac/avparse.h>

class SNYGPACMP4Track {
public:
  SNYGPACMP4Track(SNY_TRACKTYPE type, int track_id, int track_number ,GF_DecoderConfig *dcfg, SNY_UI32 time_scale, SNY_UI32 track_sample_count);
  ~SNYGPACMP4Track();
  SNY_Result parse();
  SNY_Bool reachEnd();
public:
  SNY_TRACKTYPE type;
  int track_id;
  int track_number;
  SNY_UI32 next_sample_number;
  SNY_UI32 track_sample_count;
  SNY_UI32 time_scale;
  SNY_Bool end_of_track;
  
  /*AAC*/
  GF_DecoderConfig *dcfg;
  GF_M4ADecSpecInfo a_cfg;
  SNY_UI32 aac_mode;
  SNY_UI32 aac_type;
};

#endif /*_STREAMINGNOLOGY_CORE_SNYGPACMP4TRACK_H_*/
