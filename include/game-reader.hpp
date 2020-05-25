#pragma once

#include "dmntcht.h"
#include <functional>
#include <switch.h>
#include <vector>

#define RETURN_IF_FAIL(rc) \
  if (R_FAILED(rc))        \
    return rc;

class GameReader {
 public:
  Result RefreshMetadata() {
    m_hasMetadata = false;
    return doWithDmntchtSession([]() { return 0; });
  };
  Result GetTitleId(u64 *titleId) {
    RETURN_IF_FAIL(RefreshMetadata());

    *titleId = m_metadata.title_id;

    return 0;
  }
  Result ReadHeap(u64 offset, void *buffer, size_t size) {
    return doWithDmntchtSession([this, offset, buffer, size]() {
      return dmntchtReadCheatProcessMemory(m_metadata.heap_extents.base + offset, buffer, size);
    });
  }

 private:
  bool m_hasMetadata = false;
  DmntCheatProcessMetadata m_metadata;

  Result doWithDmntchtSession(std::function<Result()> func) {
    RETURN_IF_FAIL(dmntchtInitialize());

    if (!m_hasMetadata) {
      RETURN_IF_FAIL(dmntchtGetCheatProcessMetadata(&m_metadata));
      m_hasMetadata = true;
    }

    auto rc = func();

    dmntchtExit();

    return rc;
  }
};
