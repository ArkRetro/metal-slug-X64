/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8fdc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_c8fdc(void)

{
  uint uVar1;
  uint in_D0;
  byte bVar2;
  undefined2 uVar3;
  short sVar4;
  short sVar5;
  uint uVar6;
  uint uVar7;
  int unaff_A5;

  if (*(char *)(unaff_A5 + 0x69e1) == '\0') {
    return in_D0;
  }
  if ((ushort)in_D0 < 0x6400) {
    if (((ushort)in_D0 < 0xa00) &&
       (bVar2 = (byte)in_D0, in_D0 = in_D0 & 0xffffff0c, (bVar2 & 0xc) == 0xc)) {
      uVar1 = 0x7243;
      DAT_00300001 = 0x43;
      for (sVar4 = 4; sVar4 != 0; sVar4 = sVar4 + -1) {
        uVar6 = CONCAT22((short)uVar1,0xff);
        for (sVar5 = 2; sVar5 != 0; sVar5 = sVar5 + -1) {
          if (0x74ffffff < uVar6) {
            __m68k_trap(0xf);
          }
          uVar6 = uVar6 + 0x10000;
        }
        uVar1 = (uint)(ushort)((short)uVar1 + 0x20);
      }
      _DAT_003c0004 = 0;
      _DAT_003c0000 = 0x82000000;
      return uVar1;
    }
    candidate_3cf60();
    uVar3 = *(undefined2 *)((short)((short)((in_D0 >> 8 & 0xff) % 10) * 2) + 0xc8eb8);
    sVar4 = 2;
    uVar1 = 0x7283;
  }
  else {
    uVar1 = 0x7243;
    uVar3 = 0x5904;
    sVar4 = 4;
  }
  do {
    sVar5 = 2;
    uVar6 = CONCAT22((short)uVar1,uVar3);
    do {
      uVar7 = uVar6;
      if (0x74ffffff < uVar7) {
        __m68k_trap(0xf);
      }
      sVar5 = sVar5 + -1;
      uVar6 = CONCAT31((int3)(uVar7 + 0x10000 >> 8),(char)(uVar7 + 0x10000) + '\x10');
    } while (sVar5 != 0);
    uVar3 = CONCAT11((char)((ushort)uVar3 >> 8),(char)uVar3 + '\x01');
    uVar1 = (uint)(ushort)((short)uVar1 + 0x20);
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  _DAT_003c0000 = CONCAT22(0x8200,(short)uVar7);
  return uVar1;
}
