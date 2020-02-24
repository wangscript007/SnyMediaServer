/*****************************************************************
|
|    AP4 - Sample Descriptions
|
|    Copyright 2002-2008 Axiomatic Systems, LLC
|
|
|    This file is part of Bento4/AP4 (MP4 Atom Processing Library).
|
|    Unless you have obtained Bento4 under a difference license,
|    this version of Bento4 is Bento4|GPL.
|    Bento4|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Bento4|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Bento4|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/

#ifndef _AP4_SAMPLE_DESCRIPTION_H_
#define _AP4_SAMPLE_DESCRIPTION_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "Ap4Types.h"


const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_MAIN              = 1;  /**< AAC Main Profile                             */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_LC                = 2;  /**< AAC Low Complexity                           */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_SSR               = 3;  /**< AAC Scalable Sample Rate                     */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_LTP               = 4;  /**< AAC Long Term Predictor                      */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_SBR                   = 5;  /**< Spectral Band Replication                    */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_AAC_SCALABLE          = 6;  /**< AAC Scalable                                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_TWINVQ                = 7;  /**< Twin VQ                                      */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_CELP                  = 8;  /**< CELP                                         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_HVXC                  = 9;  /**< HVXC                                         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_TTSI                  = 12; /**< TTSI                                         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_MAIN_SYNTHETIC        = 13; /**< Main Synthetic                               */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_WAVETABLE_SYNTHESIS   = 14; /**< WavetableSynthesis                           */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_GENERAL_MIDI          = 15; /**< General MIDI                                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ALGORITHMIC_SYNTHESIS = 16; /**< Algorithmic Synthesis                        */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_AAC_LC             = 17; /**< Error Resilient AAC Low Complexity           */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_AAC_LTP            = 19; /**< Error Resilient AAC Long Term Prediction     */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_AAC_SCALABLE       = 20; /**< Error Resilient AAC Scalable                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_TWINVQ             = 21; /**< Error Resilient Twin VQ                      */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_BSAC               = 22; /**< Error Resilient Bit Sliced Arithmetic Coding */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_AAC_LD             = 23; /**< Error Resilient AAC Low Delay                */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_CELP               = 24; /**< Error Resilient CELP                         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_HVXC               = 25; /**< Error Resilient HVXC                         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_HILN               = 26; /**< Error Resilient HILN                         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_PARAMETRIC         = 27; /**< Error Resilient Parametric                   */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_SSC                   = 28; /**< SSC                                          */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_PS                    = 29; /**< Parametric Stereo                            */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_MPEG_SURROUND         = 30; /**< MPEG Surround                                */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_LAYER_1               = 32; /**< MPEG Layer 1                                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_LAYER_2               = 33; /**< MPEG Layer 2                                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_LAYER_3               = 34; /**< MPEG Layer 3                                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_DST                   = 35; /**< DST Direct Stream Transfer                   */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ALS                   = 36; /**< ALS Lossless Coding                          */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_SLS                   = 37; /**< SLS Scalable Lossless Coding                 */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_SLS_NON_CORE          = 38; /**< SLS Sclable Lossless Coding Non-Core         */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_ER_AAC_ELD            = 39; /**< Error Resilient AAC ELD                      */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_SMR_SIMPLE            = 40; /**< SMR Simple                                   */
const AP4_UI08 AP4_MPEG4_AUDIO_OBJECT_TYPE_SMR_MAIN              = 41; /**< SMR Main                                     */

#endif // _AP4_SAMPLE_DESCRIPTION_H_

