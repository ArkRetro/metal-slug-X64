/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x011cfe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_11cfe(void)

{
  short *extraout_A0;
  short *extraout_A0_00;
  short *psVar1;
  uint extraout_A0_01;
  uint extraout_A0_02;
  uint uVar2;
  int iVar3;
  short *extraout_A1;
  short *psVar4;
  uint extraout_A1_00;
  uint uVar5;

  do {
  } while (bRam00109946 != bRam00109947);
  if ((char)bRam00109946 < '\0') {
    if (0x10a94e < uRam0010ad4e) {
      candidate_113ea();
      uRam0010b95a = uRam0010b956;
      uRam0010b962 = uRam0010b95e;
      uVar5 = *(uint *)(extraout_A0_01 + 0x400);
      uVar2 = extraout_A0_01;
      while (uVar2 < uVar5) {
        iVar3 = uVar2 + (int)*(short *)(uVar5 - 4);
        (*(code *)((short)((ushort)((byte)*(undefined2 *)(iVar3 + 6) & 0xfb |
                                   -(*(short *)(iVar3 + 4) != -1) & 4U) * 4) + 0xa0c6))();
        uVar2 = extraout_A0_02;
        uVar5 = extraout_A1_00;
      }
      uRam00100014 = 1;
      _DAT_002ffff0 = 1;
      do {
      } while (DAT_002fffff != '\x01');
    }
  }
  else if (0x109948 < uRam00109d48) {
    candidate_113ea();
    uRam0010b958 = uRam0010b954;
    uRam0010b960 = uRam0010b95c;
    psVar1 = extraout_A0;
    psVar4 = extraout_A0;
    while (psVar4 < *(short **)(psVar1 + 0x200)) {
      (*(code *)((short)((ushort)((byte)*(undefined2 *)((int)psVar1 + *psVar4 + 6) & 0xfb |
                                 -(*(short *)((int)psVar1 + *psVar4 + 4) != -1) & 4U) * 4) + 0xa086)
      )();
      psVar1 = extraout_A0_00;
      psVar4 = extraout_A1;
    }
    uRam00100014 = 1;
    _DAT_002ffff0 = 1;
    do {
    } while (DAT_002fffff != '\x01');
  }
  iVar3 = 0x109948;
  if (-1 < (char)bRam00109946) {
    iVar3 = 0x10a94e;
  }
  bRam00109946 = ~bRam00109946;
  *(int *)(iVar3 + 0x400) = iVar3;
  *(undefined2 *)(candidate_1004 + iVar3) = 0x1004;
  return;
}
