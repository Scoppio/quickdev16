class sCPU : public CPU {
public:
  void enter();

  #include "core/core.hpp"
  #include "dma/dma.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"

  enum DmaState { DmaInactive, DmaRun, DmaCpuSync };

  struct {
    //core
    uint8 opcode;
    bool in_opcode;

    bool wai_lock;
    bool interrupt_pending;
    uint16 interrupt_vector;

    unsigned clock_count;
    unsigned line_clocks;

    //timing
    bool irq_lock;
    bool alu_lock;
    unsigned dram_refresh_position;

    bool nmi_valid;
    bool nmi_line;
    bool nmi_transition;
    bool nmi_pending;
    bool nmi_hold;

    bool irq_valid;
    bool irq_line;
    bool irq_transition;
    bool irq_pending;
    bool irq_hold;

    //DMA
    unsigned dma_counter;
    unsigned dma_clocks;
    bool dma_pending;
    bool hdma_pending;
    bool hdma_mode;  //0 = init, 1 = run
    DmaState dma_state;

    //MMIO

    //$2181-$2183
    uint32 wram_addr;

    //$4016-$4017
    bool joypad_strobe_latch;
    uint32 joypad1_bits;
    uint32 joypad2_bits;

    //$4200
    bool nmi_enabled;
    bool hirq_enabled, virq_enabled;
    bool auto_joypad_poll;

    //$4201
    uint8 pio;

    //$4202-$4203
    uint8 mul_a, mul_b;

    //$4204-$4206
    uint16 div_a;
    uint8  div_b;

    //$4207-$420a
    uint16 hirq_pos, virq_pos;

    //$4214-$4217
    uint16 r4214;
    uint16 r4216;

    //$4218-$421f
    uint8 joy1l, joy1h;
    uint8 joy2l, joy2h;
    uint8 joy3l, joy3h;
    uint8 joy4l, joy4h;
  } status;

  void power();
  void reset();
  void triggerIRQ();

  sCPU();
  ~sCPU();
};
