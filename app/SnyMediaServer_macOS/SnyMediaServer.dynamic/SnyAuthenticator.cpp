//
//  SnyAuthenticator.cpp
//  SnyMediaServer.dynamic
//
//  Created by developer on 8/5/19.
//

#include "SnyAuthenticator.h"

SNYUserPassword::SNYUserPassword(std::string user, std::string password) { 
  this->user = user;
  this->password = password;
}

SNYUserPassword::~SNYUserPassword() {
  
}
