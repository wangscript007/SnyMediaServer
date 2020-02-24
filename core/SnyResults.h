#ifndef _STREAMINGNOLOGY_CORE_RESULTS_H_
#define _STREAMINGNOLOGY_CORE_RESULTS_H_

const int SNY_SUCCESS                               =  0;
const int SNY_FAILURE                               = -1;
const int SNY_ERROR_OUT_OF_MEMORY                   = -2;
const int SNY_ERROR_INVALID_PARAMETERS              = -3;
const int SNY_ERROR_NO_SUCH_FILE                    = -4;
const int SNY_ERROR_PERMISSION_DENIED               = -5;
const int SNY_ERROR_CANNOT_OPEN_FILE                = -6;
const int SNY_ERROR_EOS                             = -7;
const int SNY_ERROR_WRITE_FAILED                    = -8;
const int SNY_ERROR_READ_FAILED                     = -9;
const int SNY_ERROR_INVALID_FORMAT                  = -10;
const int SNY_ERROR_NO_SUCH_ITEM                    = -11;
const int SNY_ERROR_OUT_OF_RANGE                    = -12;
const int SNY_ERROR_INTERNAL                        = -13;
const int SNY_ERROR_INVALID_STATE                   = -14;
const int SNY_ERROR_LIST_EMPTY                      = -15;
const int SNY_ERROR_LIST_OPERATION_ABORTED          = -16;
const int SNY_ERROR_INVALID_RTP_CONSTRUCTOR_TYPE    = -17;
const int SNY_ERROR_NOT_SUPPORTED                   = -18;
const int SNY_ERROR_INVALID_TRACK_TYPE              = -19;
const int SNY_ERROR_INVALID_RTP_PACKET_EXTRA_DATA   = -20;
const int SNY_ERROR_BUFFER_TOO_SMALL                = -21;
const int SNY_ERROR_NOT_ENOUGH_DATA                 = -22;
const int SNY_ERROR_NOT_ENOUGH_SPACE                = -23;

const char* SNYResult2Text(int result);

#define SNYFAILED(result) ((result) != SNY_SUCCESS)
#define SNYSUCCEEDED(result) ((result) == SNY_SUCCESS)

#endif // _STREAMINGNOLOGY_CORE_RESULTS_H_
