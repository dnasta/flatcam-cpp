#include "core/OriginalBugs.h"

#include <atomic>

namespace flatcam {

namespace {
std::atomic<bool> gReproduce{false};
}

bool reproduceOriginalBugs() { return gReproduce.load(std::memory_order_relaxed); }

void setReproduceOriginalBugs(bool on) { gReproduce.store(on, std::memory_order_relaxed); }

}  // namespace flatcam
