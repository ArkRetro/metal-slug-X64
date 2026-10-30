/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2730.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2730(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xfe;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xfd;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xf7;
  *(byte *)(unaff_A6[3] + 0x76) =
       *(byte *)(unaff_A6[3] + 0x76) &
       ~('\x01' << (uint)((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0));
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  unaff_A6[0x14] = 0;
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  candidate_4256();
  *unaff_A6 = 0xd27b0;
  candidate_d3220();
  candidate_427a();
  return;
}
