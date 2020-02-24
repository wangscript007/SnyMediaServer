#include "SnyResults.h"

const char *SNYResult2Text(int result){
    switch (result) {
        case SNY_SUCCESS:                               return "SUCCESS";
        case SNY_FAILURE:                               return "FAILURE";
        case SNY_ERROR_OUT_OF_MEMORY:                   return "ERROR_OUT_OF_MEMORY";
        case SNY_ERROR_INVALID_PARAMETERS:              return "ERROR_INVALID_PARAMETERS";
        case SNY_ERROR_NO_SUCH_FILE:                    return "ERROR_NO_SUCH_FILE";
        case SNY_ERROR_PERMISSION_DENIED:               return "ERROR_PERMISSION_DENIED";
        case SNY_ERROR_CANNOT_OPEN_FILE:                return "ERROR_CANNOT_OPEN_FILE";
        case SNY_ERROR_EOS:                             return "ERROR_EOS";
        case SNY_ERROR_WRITE_FAILED:                    return "ERROR_WRITE_FAILED";
        case SNY_ERROR_READ_FAILED:                     return "ERROR_READ_FAILED";
        case SNY_ERROR_INVALID_FORMAT:                  return "ERROR_INVALID_FORMAT";
        case SNY_ERROR_NO_SUCH_ITEM:                    return "ERROR_NO_SUCH_ITEM";
        case SNY_ERROR_OUT_OF_RANGE:                    return "ERROR_OUT_OF_RANGE";
        case SNY_ERROR_INTERNAL:                        return "ERROR_INTERNAL";
        case SNY_ERROR_INVALID_STATE:                   return "ERROR_INVALID_STATE";
        case SNY_ERROR_LIST_EMPTY:                      return "ERROR_LIST_EMPTY";
        case SNY_ERROR_LIST_OPERATION_ABORTED:          return "ERROR_LIST_OPERATION_ABORTED";
        case SNY_ERROR_INVALID_RTP_CONSTRUCTOR_TYPE:    return "ERROR_INVALID_RTP_CONSTRUCTOR_TYPE";
        case SNY_ERROR_NOT_SUPPORTED:                   return "ERROR_NOT_SUPPORTED";
        case SNY_ERROR_INVALID_TRACK_TYPE:              return "ERROR_INVALID_TRACK_TYPE";
        case SNY_ERROR_INVALID_RTP_PACKET_EXTRA_DATA:   return "ERROR_INVALID_RTP_PACKET_EXTRA_DATA";
        case SNY_ERROR_BUFFER_TOO_SMALL:                return "ERROR_BUFFER_TOO_SMALL";
        case SNY_ERROR_NOT_ENOUGH_DATA:                 return "ERROR_NOT_ENOUGH_DATA";
        default:                                        return "UNKNOWN";
    }
}
