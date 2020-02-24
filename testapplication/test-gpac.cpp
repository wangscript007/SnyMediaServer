#include <gpac/tools.h>
#include <gpac/isomedia.h>
#include <gpac/mpeg4_odf.h>
#include <gpac/bitstream.h>
#include <errno.h>
#include <stdio.h>
#include "test-gpac.h"
#include <string>
#include "databuffer.h"
#include "iostream.h"

#define MEDIA_TYPE(type, subtype) (((u64)(type)<<32)+(subtype))
static void write_adts_header(FileIOStream* writer, GF_BitStream *bs);
static void process_samples_from_track(GF_ISOFile *movie, u32 track_id, u32 *sample_index, FileIOStream* writer);
static void process_audio_samples_from_track(GF_ISOFile *movie, u32 track_id, u32 *sample_index, FileIOStream* writer);

static DataBuffer extraData;

void test_gpac() {
  std::string out = "/Users/neulion/Downloads/im.mp4.2.aac";
  FileIOStream writer;
  bool ok = writer.open(out.c_str(), FileIOStream::ReadWrite);
  if (!ok) {
    return;
  }
  std::string file = "/Users/neulion/Downloads/in.mp4";
  GF_ISOFile* f;
  u32 i, j, track_count, avc_track_count, aac_track_count;
  u32 file_type = gf_isom_probe_file(file.c_str());
#ifdef MP4_DEBUG
  gf_log_set_tool_level(GF_LOG_CONTAINER,GF_LOG_DEBUG);
#endif
  
  if((f = gf_isom_open(file.c_str(), GF_ISOM_OPEN_READ, NULL)) == NULL) {
    return;
  }
  
  track_count = gf_isom_get_track_count(f);
  
  avc_track_count = 0;
  aac_track_count = 0;
  
  for(i = 0; i < track_count; i++) {
    const u32 type = gf_isom_get_media_type(f, i + 1);
    const u32 subtype = gf_isom_get_media_subtype(f, i + 1, 1);
    const u32 mpeg4_subtype = gf_isom_get_mpeg4_subtype(f, i + 1, 1);
    printf ("Track %d, type=%c%c%c%c subtype=%c%c%c%c\n", i+1, (unsigned char) (type>>24%0x100),
            (unsigned char) ((type>>16)%0x100),(unsigned char) ((type>>8)%0x100),(unsigned char) (type%0x100),
            (unsigned char) (subtype>>24%0x100),
            (unsigned char) ((subtype>>16)%0x100),(unsigned char) ((subtype>>8)%0x100),(unsigned char) (subtype%0x100));
    if (type == GF_ISOM_MEDIA_AUDIO || mpeg4_subtype == GF_4CC( 'm', 'p', '4', 'a' ))
      aac_track_count++;
    if (type == GF_ISOM_MEDIA_VISUAL && subtype == GF_ISOM_SUBTYPE_AVC_H264)
      avc_track_count++;
    }
  
  printf("MP4: found %u tracks: %u avc and %u cc\n", track_count, avc_track_count, aac_track_count);
  extraData.clear();
  for (i = 0; i < track_count; i++) {
    const u32 type = gf_isom_get_media_type(f, i + 1);
    const u32 subtype = gf_isom_get_media_subtype(f, i + 1, 1);
    printf("Processing track %d, type=%c%c%c%c subtype=%c%c%c%c\n", i + 1,
           (unsigned char)(type >> 24 % 0x100), (unsigned char)((type >> 16) % 0x100),
           (unsigned char)((type >> 8) % 0x100), (unsigned char)(type % 0x100),
           (unsigned char)(subtype >> 24 % 0x100), (unsigned char)((subtype >> 16) % 0x100),
           (unsigned char)((subtype >> 8) % 0x100), (unsigned char)(subtype % 0x100));
    
    const u64 track_type = MEDIA_TYPE(type, subtype);
    const char prefix[] = {0x00,0x00,0x00,0x01};
    switch (track_type) {
      case MEDIA_TYPE(GF_ISOM_MEDIA_VISUAL, GF_ISOM_SUBTYPE_AVC_H264): {
      GF_AVCConfig *cnf = gf_isom_avc_config_get(f, i + 1, 1);
      if (cnf != NULL) {
        for (j = 0; j < gf_list_count(cnf->sequenceParameterSets); j++) {
          GF_AVCConfigSlot* seqcnf = (GF_AVCConfigSlot*)gf_list_get(cnf->sequenceParameterSets, j);
          extraData.append(prefix, sizeof(prefix));
          extraData.append(seqcnf->data, seqcnf->size);
          printf("%d %s", seqcnf->id, seqcnf->data);
        }
        for (j = 0; j < gf_list_count(cnf->pictureParameterSets); j++) {
          GF_AVCConfigSlot* ppscnf = (GF_AVCConfigSlot*)gf_list_get(cnf->pictureParameterSets, j);
          extraData.append(prefix, sizeof(prefix));
          extraData.append(ppscnf->data, ppscnf->size);
          printf("%d %s", ppscnf->id, ppscnf->data);
        }
      }
      }
        break;
    }
    //writer.write(extraData.data(), extraData.size());
  }
  
  u32 track_id = 1;
  u32 sample_index = 1;
  //process_samples_from_track(f, 1, &sample_index, &writer);
  process_audio_samples_from_track(f, 2, &sample_index, &writer);
  printf("\nClosing media: ");
  gf_isom_close(f);
  f = NULL;
  printf("ok\n");
  
  if(avc_track_count)
    printf("Found %d AVC track(s). ", avc_track_count);
  else
    printf("Found no AVC track(s). ");
  
  if (aac_track_count)
    printf("Found %d CC track(s).\n", aac_track_count);
  else
    printf("Found no dedicated CC track(s).\n");
  
  return;
}

static void process_samples_from_track(GF_ISOFile *movie, u32 track_id, u32 *sample_index, FileIOStream* writer) {
  u32 track_number;
  u32 sample_count;
  /* Error indicator */
  GF_Err e;
  /* Number of bytes required to finish the current ISO Box reading */
  u64 missing_bytes;
  
  track_number = gf_isom_get_track_by_id(movie, track_id);
  if (track_number == 0) {
    fprintf(stdout, "Could not find track ID=%u. Ignore segment.\n", track_id);
    return;
  }
  e = gf_isom_set_nalu_extract_mode(movie, track_number, GF_ISOM_NALU_EXTRACT_TILE_ONLY | GF_ISOM_NALU_EXTRACT_ANNEXB_FLAG | GF_ISOM_NALU_EXTRACT_INBAND_PS_FLAG);
  if (e != GF_OK) {
    return;
  }
  
  u64 desiredTime = 10* 1000;
  u32 streamDescIndex; GF_ISOSample * sample = NULL; u32 num;
  e = gf_isom_get_sample_for_movie_time(movie, track_number, desiredTime, &streamDescIndex, GF_ISOM_SEARCH_FORWARD, &sample, &num);

  sample_count = gf_isom_get_sample_count(movie, track_number);
  while (num <= sample_count) {
    GF_ISOSample *iso_sample;
    u32 sample_description_index;
    iso_sample = gf_isom_get_sample(movie, track_number, num, &sample_description_index);
    if (iso_sample) {
      fprintf(stdout, "Found sample #%5d/%5d of length %8d, RAP: %d, DTS: " LLD", CTS: " LLD"\n", num, sample_count, iso_sample->dataLength,iso_sample->IsRAP, iso_sample->DTS, iso_sample->DTS+iso_sample->CTS_Offset);
      (num)++;
      if (iso_sample->IsRAP) {
        //writer->write(extraData.data(), extraData.size());
      }
      writer->write(iso_sample->data, iso_sample->dataLength);
      gf_isom_sample_del(&iso_sample);
    } else {
      e = gf_isom_last_error(movie);
      if (e == GF_ISOM_INCOMPLETE_FILE) {
        missing_bytes = gf_isom_get_missing_bytes(movie, track_number);
        fprintf(stdout, "Missing " LLU" bytes on input file\n", missing_bytes);
        gf_sleep(1000);
      }
    }
  }

  /*
  while (*sample_index <= sample_count) {
    GF_ISOSample *iso_sample;
    u32 sample_description_index;
    iso_sample = gf_isom_get_sample(movie, track_number, *sample_index, &sample_description_index);
    if (iso_sample) {
      fprintf(stdout, "Found sample #%5d/%5d of length %8d, RAP: %d, DTS: " LLD", CTS: " LLD"\n", *sample_index, sample_count, iso_sample->dataLength,iso_sample->IsRAP, iso_sample->DTS, iso_sample->DTS+iso_sample->CTS_Offset);
      (*sample_index)++;
      if (iso_sample->IsRAP) {
        writer->write(extraData.data(), extraData.size());
      }
      GF_BitStream *bs = gf_bs_new(iso_sample->data, iso_sample->dataLength, GF_BITSTREAM_READ);
      u32 size = gf_bs_read_int(bs, 32);
      u32 offset = 0;
      while (size > 0) {
        const char prefix[] = {0x00,0x00,0x00,0x01};
        writer->write(prefix, sizeof(prefix));
        offset += 4;
        writer->write(iso_sample->data + offset, size);
        offset += size;
        gf_bs_skip_bytes(bs, size);
        size = gf_bs_read_int(bs, 32);
      }
      gf_bs_del(bs);
   
      gf_isom_sample_del(&iso_sample);
    } else {
      e = gf_isom_last_error(movie);
      if (e == GF_ISOM_INCOMPLETE_FILE) {
        missing_bytes = gf_isom_get_missing_bytes(movie, track_number);
        fprintf(stdout, "Missing " LLU" bytes on input file\n", missing_bytes);
        gf_sleep(1000);
      }
    }
  }
  */
}

#include "gpac/avparse.h"
#include "gpac/constants.h"
static void process_audio_samples_from_track(GF_ISOFile *movie, u32 track_id, u32 *sample_index, FileIOStream* writer) {
  u32 track_number;
  u32 sample_count;
  /* Error indicator */
  GF_Err e;
  /* Number of bytes required to finish the current ISO Box reading */
  u64 missing_bytes;
  
  track_number = gf_isom_get_track_by_id(movie, track_id);
  if (track_number == 0) {
    fprintf(stdout, "Could not find track ID=%u. Ignore segment.\n", track_id);
    return;
  }
  e = gf_isom_set_nalu_extract_mode(movie, track_number, GF_ISOM_NALU_EXTRACT_TILE_ONLY | GF_ISOM_NALU_EXTRACT_ANNEXB_FLAG | GF_ISOM_NALU_EXTRACT_INBAND_PS_FLAG);
  if (e != GF_OK) {
    return;
  }
  GF_DecoderConfig *dcfg = NULL; u32 dsi_size = 0; char *dsi = NULL;
  dcfg = gf_isom_get_decoder_config(movie, track_number, 1);
  u32 aac_type, aac_mode;
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
        return;//return GF_NON_COMPLIANT_BITSTREAM;
      }
      dsi = dcfg->decoderSpecificInfo->data;
      dcfg->decoderSpecificInfo->data = NULL;
      dsi_size = dcfg->decoderSpecificInfo->dataLength;
      aac_mode = 2;
      break;
      
    default:
      break;
  }
  
  GF_M4ADecSpecInfo a_cfg;
  gf_m4a_get_config(dsi, dsi_size, &a_cfg);
  
  u64 desiredTime = 0;
  u32 streamDescIndex; GF_ISOSample * sample = NULL; u32 num;
  e = gf_isom_get_sample_for_media_time(movie, track_number, desiredTime, &streamDescIndex, GF_ISOM_SEARCH_FORWARD, &sample, &num);
  
  sample_count = gf_isom_get_sample_count(movie, track_number);
  while (num <= sample_count) {
    GF_ISOSample *iso_sample;
    u32 sample_description_index;
    iso_sample = gf_isom_get_sample(movie, track_number, num, &sample_description_index);
    if (iso_sample) {
      fprintf(stdout, "Found sample #%5d/%5d of length %8d, RAP: %d, DTS: " LLD", CTS: " LLD"\n", num, sample_count, iso_sample->dataLength,iso_sample->IsRAP, iso_sample->DTS, iso_sample->DTS+iso_sample->CTS_Offset);
      (num)++;
      
      GF_BitStream *bs;
      bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
      gf_bs_write_int(bs, 0xFFF, 12);/*sync*/
      gf_bs_write_int(bs, (aac_mode==1) ? 1 : 0, 1);/*mpeg2 aac*/
      gf_bs_write_int(bs, 0, 2); /*layer*/
      gf_bs_write_int(bs, 1, 1); /* protection_absent*/
      gf_bs_write_int(bs, aac_type, 2);
      gf_bs_write_int(bs, a_cfg.base_sr_index, 4);
      gf_bs_write_int(bs, 0, 1);
      gf_bs_write_int(bs, a_cfg.nb_chan, 3);
      gf_bs_write_int(bs, 0, 4);
      gf_bs_write_int(bs, 7+iso_sample->dataLength, 13);
      gf_bs_write_int(bs, 0x7FF, 11);
      gf_bs_write_int(bs, 0, 2);
      //gf_bs_write_data(bs, iso_sample->data, iso_sample->dataLength);
      
      write_adts_header(writer, bs);
      gf_bs_del(bs);

      
      writer->write(iso_sample->data, iso_sample->dataLength);
      gf_isom_sample_del(&iso_sample);
    } else {
      e = gf_isom_last_error(movie);
      if (e == GF_ISOM_INCOMPLETE_FILE) {
        missing_bytes = gf_isom_get_missing_bytes(movie, track_number);
        fprintf(stdout, "Missing " LLU" bytes on input file\n", missing_bytes);
        gf_sleep(1000);
      }
    }
  }
}

static void write_adts_header(FileIOStream* writer, GF_BitStream *bs) {
  char *data = NULL; u32 size = 0;
  if (bs != NULL) {
    gf_bs_get_content(bs, &data, &size);
    //gf_bs_get_content_no_truncate(bs, &data, &size, &allocSz);
    if (data != NULL && size > 0) {
      writer->write(data, size);
    }
  }
}
