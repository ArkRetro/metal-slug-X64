/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0260c8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_260c8(void)

{
  char cVar1;
  short sVar2;
  ushort uVar3;
  byte bVar4;
  ushort unaff_D7w;
  char *in_A0;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;

  candidate_260a6();
  bVar4 = *(byte *)(unaff_A5 + 0x7d8b);
  if (*(char *)(unaff_A5 + -0x7f78) != '\x02') {
    bVar4 = 3;
  }
  if ((6 < bVar4) && (1 < unaff_D7w)) {
    unaff_D7w = CONCAT11((char)(unaff_D7w >> 8),1);
  }
  cVar1 = *(char *)((short)((short)(1 << (bVar4 & 7) + 1) - 1U & (ushort)(byte)in_A0[1]) + 0x25fa6);
  if (cVar1 == '\0') {
    __m68k_trap(0xf);
  }
  sVar2 = unaff_D7w + (ushort)(byte)(cVar1 - 1) * 2;
  if (*in_A0 != '\x02') {
    uVar3 = CONCAT11((char)((ushort)(sVar2 * 4) >> 8),*(undefined1 *)(unaff_A6 + 0x68));
    if (0xb < uVar3) {
      __m68k_trap(0xf);
    }
    return *(undefined2 *)(in_A0 + (short)(uVar3 * 2 + sVar2 * 0xc) + 2);
  }
  if (0xb < CONCAT11((char)((ushort)(sVar2 * 2) >> 8),*(undefined1 *)(unaff_A6 + 0x68))) {
    __m68k_trap(0xf);
  }
  uVar3 = candidate_29c6();
  return *(undefined2 *)(extraout_A0 + (short)(uVar3 & 0xe));
}
