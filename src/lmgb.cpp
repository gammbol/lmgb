#include <lmgb.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: lmgb <path-to-game-file>" << std::endl;
    return -1;
  }

  // TODO: file extension validation

  try {
    lmgb::gb console{argv[1]};

    while (true) {
      console.step();
    }
  } catch (std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}

namespace lmgb {

gb::gb(const char *path) : 
  rom_data(),
  ,
  cpu_{memory_}
{
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

  if (mbc_type != MBC_TYPES::MBC1_RAM) {
    ram_size = RAM_SIZES::RAM_NO_RAM;
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
  memory_ = new mem{mbc_type, rom_size, ram_size, rom_data, pixel_processing_unit_};
  cpu_ = new cpu{memory_};
  renderer_ = new renderer(memory_, game_title, vertex_path, fragment_path);
}

void gb::sync_devices(const unsigned cycles) {
  if (cycles == 0) return;

  pixel_processing_unit_.step(cycles);
  timer_.step(cycles, interrupt_handler_);
}

void gb::step() {
#ifdef LMGB_DEBUG
  std::cout << "============ CPU STATE ============" << std::endl;
  std::cout << "AF: " << std::hex << (int)cpu_.af.bytes.h << " " << (int)cpu_.af.bytes.l << std::endl;
  std::cout << "BC: " << std::hex << (int)cpu_.bc.bytes.h << " " << (int)cpu_.bc.bytes.l << std::endl;
  std::cout << "DE: " << std::hex << (int)cpu_.de.bytes.h << " " << (int)cpu_.de.bytes.l << std::endl;
  std::cout << "HL: " << std::hex << (int)cpu_.hl.bytes.h << " " << (int)cpu_.hl.bytes.l << std::endl;
  std::cout << "===================================" << std::endl;
#endif

  word cycles = cpu_.state == HALTED ? CYCLES_WHILE_HALTED : cpu_.step();
  sync_devices(cycles);

  bool interrupt_serviced = interrupt_handler_.step(cpu_);
  if (interrupt_serviced) sync_devices(SERVICING_INTERRUPT_CYCLES);
}

// lmgb::gb::~gb() { delete rom_data; }
gb::~gb() {
  delete renderer_;
}

}