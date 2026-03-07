#include <lmgb.h>

lmgb::gb::gb(const char *path) : rom_data() {
  std::ifstream game_data;

  game_data.open(path);
  if (!game_data.is_open()) {
    throw std::invalid_argument("Unable to open file");
  }

  byte tmp;

  // reading game title
  game_data.seekg(0x0134, std::ifstream::beg);
  game_data.read(game_title, sizeof(game_title));

  // reading MBC type
  game_data.seekg(0x0147, std::ifstream::beg);
  game_data.read(reinterpret_cast<char *>(&tmp), 1);
  mbc_type = static_cast<MBC_TYPES>(tmp);

  // reading ROM size
  game_data.seekg(0x0148, std::ifstream::beg);
  game_data.read(reinterpret_cast<char *>(&tmp), 1);
  rom_size = static_cast<ROM_SIZES>(tmp);

  if (mbc_type != lmgb::MBC_TYPES::MBC1_RAM) {
    ram_size = lmgb::RAM_SIZES::RAM_NO_RAM;
  } else {
    game_data.seekg(0x0149, std::ifstream::beg);
    game_data.read(reinterpret_cast<char *>(&tmp), 1);
    ram_size = static_cast<RAM_SIZES>(tmp);
  }

  // reserving space for rom_data vector
  std::cout << (int)mbc_type << std::endl;
  std::cout << (int)rom_size << std::endl;
  std::cout << (int)ram_size << std::endl;
  std::cout << game_title << std::endl;
  switch (rom_size) {
  case ROM_KIB_32:
    std::cout << "this game is 32kib" << std::endl;
    rom_data.resize(32 * 1024);
    break;
  case ROM_KIB_64:
    std::cout << "this game is 64kib" << std::endl;
    rom_data.resize(64 * 1024);
    break;
  case ROM_KIB_128:
    std::cout << "this game is 128kib" << std::endl;
    rom_data.resize(128 * 1024);
    break;
  case ROM_KIB_256:
    std::cout << "this game is 256kib" << std::endl;
    rom_data.resize(256 * 1024);
    break;
  case ROM_KIB_512:
    std::cout << "this game is 512kib" << std::endl;
    rom_data.resize(512 * 1024);
    break;
  }

  std::cout << "SIZE: " << rom_data.size() << std::endl;

  // get game file size
  game_data.seekg(std::ifstream::end);
  std::streampos file_size = game_data.tellg();
  game_data.seekg(std::ifstream::beg);

  // reading the whole file
  game_data.read(reinterpret_cast<char *>(rom_data.data()), file_size);
}

// lmgb::gb::~gb() { delete rom_data; }

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
