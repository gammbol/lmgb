#include <synchronizer.h>

void lmgb::synchronizer::sync_with_cpu(int delta, cpu &cpu) {
  gpu.step(delta);
  intr.step(delta, cpu);
}