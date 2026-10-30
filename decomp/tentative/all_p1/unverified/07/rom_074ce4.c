/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074ce4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_74ce4(void)

{
  uint uVar1;
  uint *extraout_A0;
  int unaff_A6;

  candidate_ca2ce();
  uVar1 = *extraout_A0;
  *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) & 0xfd;
  if ((uVar1 >> 0x10 & 8) != 0) {
    *(byte *)(unaff_A6 + 0x8e) = *(byte *)(unaff_A6 + 0x8e) | 2;
  }
  *(byte *)(unaff_A6 + 0x95) =
       *(byte *)(unaff_A6 + 0x96) & (byte)(uVar1 >> 0x10) | *(byte *)(unaff_A6 + 0x95);
  return;
}
