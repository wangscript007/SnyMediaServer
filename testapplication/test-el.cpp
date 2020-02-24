#include "easylogging++.h"

void test_el() {
    // Load configuration from file
    el::Configurations conf("/Users/developer⁩/Documents⁩/⁨sny⁩/streamingnology/log/el.conf");
    // Reconfigure single logger
    el::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    el::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file
  LOG(INFO) << "This is info log";
}
