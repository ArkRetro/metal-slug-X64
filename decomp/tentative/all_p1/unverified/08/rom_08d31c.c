/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d31c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_8d31c(void)

{
  undefined2 uVar1;
  int unaff_A5;

  uVar1 = *(undefined2 *)(unaff_A5 + 0x45ea);
  *(short *)(unaff_A5 + 0x45ea) = *(short *)(unaff_A5 + 0x45ea) + 1;
  *(short *)(unaff_A5 + 0x45ea) = *(short *)(unaff_A5 + 0x45ea) + 1;
  *(ushort *)(unaff_A5 + 0x45ea) = *(ushort *)(unaff_A5 + 0x45ea) & 7;
  return uVar1;
}
