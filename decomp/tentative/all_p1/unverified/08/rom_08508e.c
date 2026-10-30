/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08508e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8508e(void)

{
  short sVar2;
  undefined4 uVar1;
  int unaff_A6;

  candidate_260c8();
  sVar2 = candidate_260c8();
  *(int *)(unaff_A6 + 0x34) = (int)sVar2 << 8;
  uVar1 = candidate_84fae();
  *(undefined4 *)(unaff_A6 + 0x7e) = uVar1;
  sVar2 = candidate_260c8();
  if (-1 < (int)((uint)(ushort)((sVar2 + 0x100) - *(short *)(unaff_A6 + 0x40)) << 0x10)) {
    candidate_85026();
  }
  uVar1 = candidate_84ff2();
  *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
  return;
}
