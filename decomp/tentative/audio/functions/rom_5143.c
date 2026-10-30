/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x5143; tentative_c. */

void z80_candidate_5143(void)

{
  byte bVar1;
  byte bVar2;

  bVar1 = bRamfdff & 0xf7;
  if ((bRamfdff & 4) != 0) {
    bVar2 = bRamfdff & 0x40;
    bRamfdff = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfdf6);
    }
    z80_candidate_531f();
    bVar1 = bRamfdff;
  }
  bRamfdff = bVar1;
  bVar1 = bRamfe22 & 0xf7;
  if ((bRamfe22 & 4) != 0) {
    bVar2 = bRamfe22 & 0x40;
    bRamfe22 = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfe19);
    }
    z80_candidate_531f();
    bVar1 = bRamfe22;
  }
  bRamfe22 = bVar1;
  bVar1 = bRamfe45 & 0xf7;
  if ((bRamfe45 & 4) != 0) {
    bVar2 = bRamfe45 & 0x40;
    bRamfe45 = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfe3c);
    }
    z80_candidate_531f();
    bVar1 = bRamfe45;
  }
  bRamfe45 = bVar1;
  bVar1 = bRamfe68 & 0xf7;
  if ((bRamfe68 & 4) != 0) {
    bVar2 = bRamfe68 & 0x40;
    bRamfe68 = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfe5f);
    }
    z80_candidate_531f();
    bVar1 = bRamfe68;
  }
  bRamfe68 = bVar1;
  bVar1 = bRamfe8b & 0xf7;
  if ((bRamfe8b & 4) != 0) {
    bVar2 = bRamfe8b & 0x40;
    bRamfe8b = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfe82);
    }
    z80_candidate_531f();
    bVar1 = bRamfe8b;
  }
  bRamfe8b = bVar1;
  bVar1 = bRamfeae & 0xf7;
  if ((bRamfeae & 4) != 0) {
    bVar2 = bRamfeae & 0x40;
    bRamfeae = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfea5);
    }
    z80_candidate_531f();
    bVar1 = bRamfeae;
  }
  bRamfeae = bVar1;
  bVar1 = bRamfed1 & 0xf7;
  if ((bRamfed1 & 4) != 0) {
    bVar2 = bRamfed1 & 0x40;
    bRamfed1 = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfec8);
    }
    z80_candidate_531f();
    bVar1 = bRamfed1;
  }
  bRamfed1 = bVar1;
  bVar1 = bRamfef4 & 0xf7;
  if ((bRamfef4 & 4) != 0) {
    bVar2 = bRamfef4 & 0x40;
    bRamfef4 = bVar1;
    if (bVar2 != 0) {
      z80_candidate_52de(0xfeeb);
    }
    z80_candidate_531f();
    bVar1 = bRamfef4;
  }
  bRamfef4 = bVar1;
  return;
}
