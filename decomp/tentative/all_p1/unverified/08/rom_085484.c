/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085484.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_85484(void)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  undefined4 extraout_A0;
  int unaff_A6;

  if ((0xf < *(short *)(unaff_A6 + 0x3c)) && (*(short *)(unaff_A6 + 0x3c) < 0x131)) {
    if ((-1 < (char)*(byte *)(unaff_A6 + 0x8f)) &&
       (pbVar1 = (byte *)(*(int *)(unaff_A6 + 0xc) + 0x8d), bVar2 = *pbVar1,
       bVar3 = '\x01' << (*(byte *)(unaff_A6 + 0x8f) & 7), *pbVar1 = bVar2 & ~bVar3,
       (bVar2 & bVar3) != 0)) {
      candidate_1af9a();
      *(undefined4 *)(unaff_A6 + 0x6e) = extraout_A0;
      candidate_854bc();
    }
  }
  return;
}
