/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x0000; tentative_c. */

/* WARNING: This function may have set the stack pointer */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void z80_candidate_0(undefined1 param_1)

{
  disableMaskableInterrupts();
  uIo000c = 0;
  _DAT_ram_f80b = 4;
  uRamffec = 0x1e10;
  uIo0018 = param_1;
  z80_candidate_5795();
  uRamffec = 0x1e13;
  z80_candidate_1eb9();
  uRamffec = 0x1e16;
  z80_candidate_1ff0();
  uRamffec = 0x1e19;
  z80_candidate_24c6();
  uRamffec = 0x1e1c;
  z80_candidate_2c93();
  uRamffec = 0x1e1f;
  z80_candidate_386b();
  uRamffec = 0x1e22;
  z80_candidate_405a();
  uRamffec = 0x1e25;
  FUN_ram_4cab();
  uRamffec = 0x1e28;
  FUN_ram_504c();
  DAT_ram_f801 = '\0';
  DAT_ram_f80a = 0;
  DAT_ram_f802 = 0;
  DAT_ram_f803 = 1;
  uRamffec = 0x1e3a;
  uIo0008 = z80_candidate_1e71();
  setInterruptMode(1);
  enableMaskableInterrupts();
  do {
    uRamffec = 0x1e42;
    z80_candidate_1e6a();
    uRamffec = 0x1e45;
    z80_candidate_2001();
    uRamffec = 0x1e48;
    z80_candidate_2548();
    uRamffec = 0x1e4b;
    z80_candidate_2daf();
    uRamffec = 0x1e4e;
    z80_candidate_38c7();
    uRamffec = 0x1e51;
    z80_candidate_40b4();
    uRamffec = 0x1e54;
    z80_candidate_5143();
    uRamffec = 0x1e57;
    z80_candidate_4ccd();
    do {
    } while (DAT_ram_f801 == '\0');
    DAT_ram_f801 = DAT_ram_f801 + -1;
    DAT_ram_f803 = DAT_ram_f802;
    DAT_ram_f802 = 0;
  } while( true );
}
