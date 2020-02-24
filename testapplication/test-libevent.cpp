#include "http_server.h"
#include "http_headers.h"
#include "http_content_type.h"
#include "SNYVODMediaManifest.h"
#include "SNYVODChannel.h"

#include <iostream>
#include <sstream>
#include <mutex>
#include "SNYUtils.h"
#include "SNYMediaService.h"

#include "SNYMachineInfo.h"
#include "SNYMediaServer.h"
int test_libevent(){
  signal(SIGPIPE, SIG_IGN);
  
  SNYMediaServer* snyMediaServer = new SNYMediaServer();
  snyMediaServer->start();
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3 * 1000));
  }
  return 0;
}

