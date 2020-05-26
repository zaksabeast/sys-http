#include "game-reader.hpp"
#include "httplib.h"
#include "utils.hpp"
#include <memory>
#include <switch.h>

// libnx fake heap initialization
extern "C" {
#define HEAP_SIZE 0x000340000

// we aren't an applet
u32 __nx_applet_type = AppletType_None;

// setup a fake heap (we don't need the heap anyway)
char fake_heap[HEAP_SIZE];

// we override libnx internals to do a minimal init
void __libnx_initheap(void) {
  extern char *fake_heap_start;
  extern char *fake_heap_end;

  // setup newlib fake heap
  fake_heap_start = fake_heap;
  fake_heap_end = fake_heap + HEAP_SIZE;
}

void __appInit(void) {
  smInitialize();
  fsInitialize();
  fsdevMountSdmc();
  timeInitialize();
  socketInitializeDefault();
}

void __appExit(void) {
  fsdevUnmountAll();
  fsExit();
  smExit();
  timeExit();
  socketExit();
}
}

using namespace httplib;

#define SET_STATUS_FROM_RC(res, rc) \
  if (R_FAILED(rc)) {               \
    res.status = 500;               \
  } else {                          \
    res.status = 200;               \
  }

int main() {
  Server server;
  auto game = std::make_shared<GameReader>();

  server.Get("/refreshMetadata", [game](const Request &req, Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");

    SET_STATUS_FROM_RC(res, game->RefreshMetadata());
  });

  server.Get("/titleId", [game](const Request &req, Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");

    u64 titleId = 0;

    SET_STATUS_FROM_RC(res, game->GetTitleId(&titleId));

    auto titleIdHex = convertNumToHexString(titleId);

    res.set_content(titleIdHex, "text/plain");
  });

  server.Get("/readHeap", [game](const Request &req, Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");

    u64 offset = 0;
    u64 size = 0;
    std::string offsetStr = "0";
    std::string sizeStr = "4";

    if (req.has_param("offset"))
      offsetStr = req.get_param_value("offset");

    if (req.has_param("size"))
      sizeStr = req.get_param_value("size");

    try {
      offset = stoull(offsetStr, 0, 16);
      size = stoull(sizeStr, 0, 16);
    } catch (const std::invalid_argument &ia) {
      res.set_content("Invalid parameters", "text/plain");
      res.status = 400;
    }

    if (res.status != 400) {
      u8 *buffer = new u8[size];
      auto rc = game->ReadHeap(offset, buffer, size);

      if (R_FAILED(rc)) {
        res.status = 500;
      } else {
        auto hexString = convertByteArrayToHex(buffer, size);
        res.set_content(hexString, "text/plain");
      }

      delete[] buffer;
    }
  });

  server.Get("/", [](const Request &req, Response &res) { res.set_content("Health check!", "text/plain"); });

  server.listen("0.0.0.0", 8080);

  while (appletMainLoop()) {
    svcSleepThread(100000000L);
  }

  return 0;
}
