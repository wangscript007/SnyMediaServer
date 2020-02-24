//
//  idemuxer.cpp
//  MediaServer
//
//  Created by neulion on 5/3/18.
//  Copyright © 2018 streamingnology. All rights reserved.
//

#include "SnyIDemuxer.h"
#include "SnyGPACMP4.h"

SNYIDemuxer *SNYIDemuxer::createInstance() {
  SNYGPACMP4 *GPACMP4 = new SNYGPACMP4();
  return dynamic_cast<SNYIDemuxer*>(GPACMP4);
}
