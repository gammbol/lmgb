#ifndef LMGB_SYNCHRONIZER_H
#define LMGB_SYNCHRONIZER_H

#include <graphics.h>
#include <interrupts.h>
#include <cpu.h>

namespace lmgb {

class synchronizer {
  graphics gpu;
  interrupts intr;

public:
  synchronizer(graphics &gpu, interrupts &intr)
    : gpu(gpu), intr(intr) {}

  void sync_with_cpu(int delta, cpu &cpu);
};

} // namespace lmgb

#endif