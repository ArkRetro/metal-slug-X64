/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c75b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c75b2(void)

{
  short sVar1;
  short sVar2;
  int in_A0;

  sVar1 = *(short *)(in_A0 + 0x14) - (ushort)((*(byte *)(in_A0 + 0x25) & 1) != 0);
  sVar2 = *(short *)(in_A0 + 0x18) - (ushort)((*(byte *)(in_A0 + 0x25) >> 1 & 1) != 0);
  if (sVar1 != *(short *)(in_A0 + 0x84)) {
    *(short *)(in_A0 + 0x84) = sVar1;
    *(byte *)(in_A0 + 0x82) = *(byte *)(in_A0 + 0x82) | 0x80;
  }
  if (sVar2 != *(short *)(in_A0 + 0x86)) {
    *(short *)(in_A0 + 0x86) = sVar2;
    *(byte *)(in_A0 + 0x82) = *(byte *)(in_A0 + 0x82) | 0x80;
  }
  return;
}
