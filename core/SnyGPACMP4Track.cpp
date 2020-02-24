#include "SnyGPACMP4Track.h"
#include "SnyResults.h"
#include <gpac/constants.h>

SNYGPACMP4Track::SNYGPACMP4Track(SNY_TRACKTYPE type, int track_id, int track_number ,GF_DecoderConfig *dcfg, SNY_UI32 time_scale, SNY_UI32 track_sample_count) {
  this->type          = type;
  this->track_id      = track_id;
  this->track_number  = track_number;
  this->dcfg          = dcfg;
  this->time_scale    = time_scale;
  this->track_sample_count = track_sample_count;
  this->next_sample_number = 1;
  this->end_of_track = false;
}

SNYGPACMP4Track::~SNYGPACMP4Track() {
  if (dcfg != NULL) {
    gf_odf_desc_del((GF_Descriptor *) dcfg);
  }
}

SNY_Result SNYGPACMP4Track::parse() {
  if (type == SNY_AUDIO) {
    if (dcfg == NULL) {
      return SNY_FAILURE;
    } else {
      u32 dsi_size = 0; char *dsi = NULL;
      switch (dcfg->objectTypeIndication) {
        case GPAC_OTI_AUDIO_AAC_MPEG2_MP:
        case GPAC_OTI_AUDIO_AAC_MPEG2_LCP:
        case GPAC_OTI_AUDIO_AAC_MPEG2_SSRP:
          dsi = dcfg->decoderSpecificInfo->data;
          dcfg->decoderSpecificInfo->data = NULL;
          dsi_size = dcfg->decoderSpecificInfo->dataLength;
          aac_mode = 1;
          aac_type = dcfg->objectTypeIndication - GPAC_OTI_AUDIO_AAC_MPEG2_MP;
          break;
        case GPAC_OTI_AUDIO_AAC_MPEG4:
          if (!dcfg->decoderSpecificInfo) {
            gf_odf_desc_del((GF_Descriptor *) dcfg);
            return SNY_FAILURE;
          }
          dsi = dcfg->decoderSpecificInfo->data;
          dcfg->decoderSpecificInfo->data = NULL;
          dsi_size = dcfg->decoderSpecificInfo->dataLength;
          aac_mode = 2;
          break;
          
        default:
          break;
      }
      if (dsi == NULL || dsi_size <= 0) {
        return SNY_FAILURE;
      }
      GF_Err e = gf_m4a_get_config(dsi, dsi_size, &a_cfg);
      if (e != GF_OK) {
        return SNY_FAILURE;
      }
    }
  } else if (type == SNY_VIDEO) {
    return SNY_SUCCESS;
  } else {
    return SNY_FAILURE;
  }
  return SNY_SUCCESS;
}

SNY_Bool SNYGPACMP4Track::reachEnd() {
  SNY_Bool eos_track = end_of_track;
  SNY_Bool eos_count = next_sample_number > track_sample_count ? true : false;
  return eos_track | eos_count;
}
