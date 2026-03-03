#ifndef LMGB_MBC1_H
#define LMGB_MBC1_H

#include <mbc.h>

namespace lmgb {
class mbc1 : public mbc {
public:
  mbc1() { advancedMode = false; }

  byte read(word addr) override;
  void write(word addr, byte val) override;

private:
  bool advancedMode = false;

  byte *loadRom(const char *path) override;
  byte *loadRam(const char *path) override;
  void saveRam(const char *path) override;
};
} // namespace lmgb

#endif