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
  switch (rom_size) {
  case ROM_KIB_32:
    rom_data.resize(32 * 1024);
    break;
  case ROM_KIB_64:
    rom_data.resize(64 * 1024);
    break;
  case ROM_KIB_128:
    rom_data.resize(128 * 1024);
    break;
  case ROM_KIB_256:
    rom_data.resize(256 * 1024);
    break;
  case ROM_KIB_512:
    rom_data.resize(512 * 1024);
    break;
  }


  // get game file size
  game_data.seekg(0, std::ifstream::end);
  std::streampos file_size = game_data.tellg();
  game_data.seekg(0, std::ifstream::beg);

  // reading the whole file
  game_data.read(reinterpret_cast<char *>(rom_data.data()), file_size);
  cpu.emplace(mbc_type, rom_size, ram_size, rom_data);
}

void lmgb::gb::Step() {
  std::cout << "============ CPU STATE ============" << std::endl;
  std::cout << "AF: " << std::hex << cpu->af.pair << std::endl;
  std::cout << "BC: " << std::hex << cpu->bc.pair << std::endl;
  std::cout << "DE: " << std::hex << cpu->de.pair << std::endl;
  std::cout << "HL: " << std::hex << cpu->hl.pair << std::endl;
  std::cout << "===================================" << std::endl;

  cpu->Step();
}

// lmgb::gb::~gb() { delete rom_data; }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: lmgb <path-to-game-file>" << std::endl;
    return -1;
  }

  // TODO: file extension validation

  try {
    lmgb::gb console{argv[1]};

    while (true) {
      console.Step();
    }
  } catch (std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
