#include <lmgb.h>

lmgb::gb::gb(const char *path) {
  std::ifstream game_data;

  game_data.open(path);
  if (!game_data.is_open()) {
    throw std::invalid_argument("Unable to open file");
  }

  // reading game title
  game_data.seekg(0x0134, std::ifstream::beg);
  game_data.read(game_title, sizeof(game_title));

  // reading MBC type
  game_data.seekg(0x0147, std::ifstream::beg);
  game_data.read(reinterpret_cast<char *>(&mbc_type), 1);

  // reading ROM size
  game_data.seekg(0x0148, std::ifstream::beg);
  game_data.read(reinterpret_cast<char *>(&rom_size), 1);

  if (mbc_type != lmgb::MBC_TYPES::MBC1_RAM) {
    ram_size = lmgb::RAM_SIZES::RAM_NO_RAM;
  } else {
    game_data.seekg(0x0149, std::ifstream::beg);
    game_data.read(reinterpret_cast<char *>(&ram_size), 1);
  }

  // reserving space for rom_data vector
  switch (rom_size) {
  case ROM_KIB_32:
    rom_data.reserve(32 * 1024);
    break;
  case ROM_KIB_64:
    rom_data.reserve(64 * 1024);
    break;
  case ROM_KIB_256:
    rom_data.reserve(256 * 1024);
    break;
  case ROM_KIB_512:
    rom_data.reserve(512 * 1024);
    break;
  }

  // get game file size
  game_data.seekg(std::ifstream::end);
  std::streampos file_size = game_data.tellg();
  game_data.seekg(std::ifstream::beg);

  // reading the whole file
  game_data.read(reinterpret_cast<char *>(&rom_data[0]), file_size);
}

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
