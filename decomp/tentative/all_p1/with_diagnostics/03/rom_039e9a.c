/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039e9a.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_39e9a(void)

{
  char *pcVar1;
  short sVar2;
  byte bVar4;
  ushort uVar3;
  short sVar5;
  short sVar6;
  short unaff_D3w;
  int unaff_A2;
  short *unaff_A6;
  undefined1 uVar7;

  uVar3 = candidate_260c8();
  if (3 < uVar3) {
    __m68k_trap(0xf);
    pcVar1 = (char *)(unaff_A2 + 0x33 + (int)unaff_D3w);
    *pcVar1 = *pcVar1 + '\x02';
    *unaff_A6 = *unaff_A6 + -7;
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  uVar7 = false;
  bVar4 = candidate_1ae8e(*(undefined4 *)((short)(uVar3 << 2) + 0x3a01c));
  if (!(bool)uVar7) {
    if (((bVar4 & 1) != 0) && (bVar4 = candidate_1af3c(), (bool)uVar7)) {
      uVar7 = 0;
      bVar4 = bVar4 ^ 1;
    }
    if (((bVar4 & 2) != 0) && (bVar4 = candidate_1af3c(), (bool)uVar7)) {
      bVar4 = bVar4 ^ 2;
    }
  }
  if (bVar4 != 0) {
    if (bVar4 == 1) {
      return;
    }
    if (bVar4 != 2) {
      sVar2 = unaff_A6[0x1e];
      sVar5 = sRam00101500 - sVar2;
      if (sRam00101500 < sVar2) {
        sVar5 = -sVar5;
      }
      sVar6 = sRam001015b0 - sVar2;
      if (sRam001015b0 < sVar2) {
        sVar6 = -sVar6;
      }
      if (sVar5 <= sVar6) {
        return;
      }
    }
    return;
  }
  return;
}
