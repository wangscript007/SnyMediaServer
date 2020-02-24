//
//  SnyAuthenticator.hpp
//  SnyMediaServer.dynamic
//
//  Created by developer on 8/5/19.
//

#ifndef _STREAMINGNOLOGY_MEDIASERVER_SNYAUTHENTICATOR_H_
#define _STREAMINGNOLOGY_MEDIASERVER_SNYAUTHENTICATOR_H_
#include <string>
#include <vector>
#include "SnyType.h"

class SNYUserPassword {
public:
  SNYUserPassword(std::string user, std::string password);
  ~SNYUserPassword();
public:
  std::string user;
  std::string password;
};

class SNYAuthenticator {
public:
  SNYAuthenticator();
  ~SNYAuthenticator();
public:
  void setUserPass(std::vector<SNYUserPassword>);
public:
  SNY_Bool validateUserPassword(std::string user, std::string password);
  SNY_Bool validateXSNYToken(std::string token);
private:
  std::vector<SNYUserPassword> userPasswords;
};

#endif /* SnyAuthenticator_hpp */
