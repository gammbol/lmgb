#include <lmgb.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: lmgb <path-to-game-file>" << std::endl;
    return -1;
  }

  // TODO: file extension validation

  try {
    lmgb::gb cartridge_state{argv[1]};
  } catch (std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return -1;
  }


  return 0;
}
