/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x060e7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_60e7a(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A6;

  uVar1 = *(short *)(unaff_A6 + 0x6e) - *(short *)(unaff_A6 + 0x3c);
  sVar2 = *(short *)(unaff_A6 + 0x70) - *(short *)(unaff_A6 + 0x40);
  if ((uVar1 != 0) || (sVar2 != 0)) {
    *(uint *)(unaff_A6 + 0x30) = (uint)uVar1 * 0xc000;
    *(uint *)(unaff_A6 + 0x34) = (uint)(ushort)-sVar2 * -0xc000;
  }
  return;
}
