#include "cheat/cheat.hpp"

#include "memory/memory.hpp"
#include "memory/smemory/smemory.hpp"

#include "cpu/cpu.hpp"
#include "cpu/scpu/scpu.hpp"

#include "ppu/ppu.hpp"
#include "ppu/bppu/bppu.hpp"

#include "smp/smp.hpp"
#include "smp/ssmp/ssmp.hpp"

#include "dsp/dsp.hpp"
#include "dsp/sdsp/sdsp.hpp"

extern BUSCORE bus;
extern CPUCORE cpu;
extern SMPCORE smp;
extern DSPCORE dsp;
extern PPUCORE ppu;

#include "snes/snes.hpp"
