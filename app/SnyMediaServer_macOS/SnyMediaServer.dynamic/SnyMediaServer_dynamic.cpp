//
//  SnyMediaServer_dynamic.cpp
//  SnyMediaServer.dynamic
//
//  Created by neulion on 9/2/18.
//

#include <iostream>
#include "SnyMediaServer_dynamic.hpp"
#include "SnyMediaServer_dynamicPriv.hpp"

void SnyMediaServer_dynamic::HelloWorld(const char * s)
{
    SnyMediaServer_dynamicPriv *theObj = new SnyMediaServer_dynamicPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void SnyMediaServer_dynamicPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

