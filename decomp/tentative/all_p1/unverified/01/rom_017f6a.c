/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x017f6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_17f6a(void)

{
  ushort in_D0w;
  short sVar1;
  short *psVar2;

  if (0x15 < in_D0w) {
    __m68k_trap(0xf);
  }
  psVar2 = *(short **)((short)(in_D0w * 4) + 0x17d44);
  sVar1 = *psVar2;
  while (sVar1 != 8) {
    psVar2 = (short *)((int)psVar2[1] + (int)psVar2);
    sVar1 = *psVar2;
  }
  return;
}
