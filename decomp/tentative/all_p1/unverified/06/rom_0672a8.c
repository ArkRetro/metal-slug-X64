/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0672a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_672a8(void)

{
  ushort uVar1;
  int unaff_A2;

  candidate_66ac2();
  *(short *)(unaff_A2 + 4) = (ushort)*(byte *)(unaff_A2 + 0xc) * 0x40 + *(short *)(unaff_A2 + 4);
  uVar1 = (ushort)*(byte *)(unaff_A2 + 0xc) << 8;
  if (uVar1 < *(ushort *)(unaff_A2 + 4)) {
    *(ushort *)(unaff_A2 + 4) = uVar1;
  }
  return;
}
