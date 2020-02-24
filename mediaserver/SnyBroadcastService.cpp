#include "SnyPlatform.h"
#if defined(Q_OS_WIN32)
#include <winsock2.h>
extern "C" {
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
}
#else
extern "C" {
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
}
#endif // Q_OS_WIN32
#include "event2/event.h"
#include "event2/event_struct.h"
#include "easylogging++.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "SnyBroadcastService.h"

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#define SnyBSBufferSize 100
using namespace rapidjson;
SnyBroadcastService::SnyBroadcastService(SNY_UI16 port) :threads(this) {
  this->broadcastPort = port;
  this->srvSocket = -1;
  this->base = nullptr;
}

void SnyBroadcastService::onEventCallback(const int sock, short int which, void *arg) {

  SnyBroadcastService* bc = nullptr;
  if (arg != nullptr) {
    bc = (SnyBroadcastService*)arg;
  }
  char msg[SnyBSBufferSize];

  struct sockaddr_in server_sin;
  socklen_t server_sz = sizeof(server_sin);

  /* Recv the data, store the address of the sender in server_sin */
  if (recvfrom(sock, msg, sizeof(msg) - 1, 0, (struct sockaddr *) &server_sin, &server_sz) == -1) {
    //TODO:
    return;
  }

  printf("from %s:UDP%u : %s \n", inet_ntoa(server_sin.sin_addr), ntohs(server_sin.sin_port), msg);
  char terminator[] = { 't','e','r','m','i','n','a','t','o','r' };
  if (memcmp(msg, terminator, sizeof(terminator)) == 0) {
    event_base_loopbreak(bc->getBase());
  }

#if defined(Q_OS_WIN32)
  string sys = "windows";
#elif defined(Q_OS_MACX)
  string sys = "mac";
#else
  string sys = "linux";
#endif 
  StringBuffer sb;
  Writer<StringBuffer> writer(sb);
  writer.StartObject();
  writer.Key("port");
  writer.String(std::to_string(bc->getMediaServerPort()).c_str());
  writer.Key("system");
  writer.String(sys.c_str());
  writer.EndObject();
  std::string JSON = sb.GetString();

  /* Send the data back to the client */
  if (sendto(sock, JSON.data(), JSON.size(), 0, (struct sockaddr *) &server_sin, server_sz) == -1) {
    //perror("sendto()");
    //event_loopbreak();
    //TODO:
    return;
  }
}

void SnyBroadcastService::stop() {
  if (base != nullptr) {
    terminateLoop();
  }
  threads.stop(1);
  threads.wait(1);
  if (srvSocket != -1) {
#if defined(Q_OS_WIN32)
  closesocket(srvSocket);
#else
  close(srvSocket);
#endif 
  }
}

void SnyBroadcastService::start() {
  threads.start(1);
}

SnyBroadcastService::~SnyBroadcastService() {

}

int SnyBroadcastService::onThreadProc(int id) {
  struct event udp_event;
  struct sockaddr_in sin;

  srvSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (srvSocket < 0) {
    //TODO:
    return id;
  }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(broadcastPort);
  if (::bind(srvSocket, (const struct sockaddr *) &sin, (socklen_t)(sizeof(struct sockaddr_in))) == SOCKET_ERROR) {
    //TODO:
    return id;
  }

  base = event_base_new();
  /* Initalize one event */
  event_assign(&udp_event, base, srvSocket, EV_READ | EV_PERSIST, onEventCallback, this);
  event_add(&udp_event, NULL);
  event_base_dispatch(base);
  event_base_free(base);
  base = nullptr;
  return id;
}

void SnyBroadcastService::terminateLoop() {
  int sd, rc;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *h;
  int broadcast = 1;

  h = gethostbyname("255.255.255.255");
  if (h == NULL) {
    return;
  }

  remoteServAddr.sin_family = h->h_addrtype;
  memcpy((char *)&remoteServAddr.sin_addr.s_addr,
    h->h_addr_list[0], h->h_length);
  remoteServAddr.sin_port = htons(broadcastPort);

  /* socket creation */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    return;
  }
#if defined(Q_OS_WIN32)
  if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof broadcast) == -1) {
    return;
  }
#else
  if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
    return;
  }
#endif // defined(Q_OS_WIN32)

  /* bind any port */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);

  rc = ::bind(sd, (const struct sockaddr *) &cliAddr, (socklen_t)(sizeof(struct sockaddr_in)));
  if (rc < 0) {
    return;
  }

  char msg[] = { 't','e','r','m','i','n','a','t','o','r' };
  sendto(sd, msg, size(msg), 0,
    (struct sockaddr *) &remoteServAddr,
    sizeof(remoteServAddr));
}
