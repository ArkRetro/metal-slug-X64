/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e19c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5e19c(void)

{
  int iVar1;
  int unaff_A6;

  *(char *)(unaff_A6 + 0x87) = *(char *)(unaff_A6 + 0x87) + -2;
  iVar1 = *(short *)((short)((ushort)*(byte *)(unaff_A6 + 0x87) * 2) + 0x2a7c) * 0xb0;
  *(uint *)(unaff_A6 + 0x30) =
       CONCAT22((short)((uint)iVar1 >> 0x10),*(short *)(unaff_A6 + 0x7e) + (short)iVar1);
  *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(unaff_A6 + 0x62);
  func_0x000143e4();
  return;
}
