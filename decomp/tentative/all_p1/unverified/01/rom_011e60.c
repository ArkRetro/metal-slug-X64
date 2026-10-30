/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x011e60.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_11e60(void)

{
  int iVar1;
  short *extraout_A0;
  short *psVar2;
  uint extraout_A0_00;
  uint uVar3;
  short *extraout_A1;
  short *psVar4;
  uint extraout_A1_00;
  uint uVar5;

  if (cRam00109946 != cRam00109947) {
    cRam00109947 = cRam00109946;
    if (cRam00109946 < '\0') {
      psVar2 = (short *)0x109948;
      uRam0010b958 = uRam0010b954;
      uRam0010b960 = uRam0010b95c;
      psVar4 = psVar2;
      while (psVar4 < *(short **)(psVar2 + 0x200)) {
        (*(code *)((short)((ushort)((byte)*(undefined2 *)((int)psVar2 + *psVar4 + 6) & 0xfb |
                                   -(*(short *)((int)psVar2 + *psVar4 + 4) != -1) & 4U) * 4) +
                  0xa0a6))();
        psVar2 = extraout_A0;
        psVar4 = extraout_A1;
      }
      candidate_103a6();
      candidate_103a6();
      do {
      } while (DAT_002fffff != '\x01');
    }
    else {
      uVar3 = 0x10a94e;
      uRam0010b95a = uRam0010b956;
      uRam0010b962 = uRam0010b95e;
      uVar5 = uRam0010ad4e;
      while (uVar3 < uVar5) {
        iVar1 = uVar3 + (int)*(short *)(uVar5 - 4);
        (*(code *)((short)((ushort)((byte)*(undefined2 *)(iVar1 + 6) & 0xfb |
                                   -(*(short *)(iVar1 + 4) != -1) & 4U) * 4) + 0xa0e6))();
        uVar3 = extraout_A0_00;
        uVar5 = extraout_A1_00;
      }
      candidate_10bc8();
      candidate_10bc8();
      do {
      } while (DAT_002fffff != '\x01');
    }
    _DAT_002ffff0 = 1;
    uRam00100014 = 1;
    return;
  }
  return;
}
