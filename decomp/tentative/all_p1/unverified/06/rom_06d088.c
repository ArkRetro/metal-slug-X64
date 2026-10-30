/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d088.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d088(void)

{
  ushort uVar1;
  int unaff_A6;
  undefined1 in_CF;
  undefined8 uVar2;

  candidate_6d126();
  if (!(bool)in_CF) {
    *(int *)(unaff_A6 + 0x34) = *(int *)(unaff_A6 + 0x34) + -0x8000;
    func_0x000143e4();
    return;
  }
  uVar1 = (short)*(char *)(unaff_A6 + 0x7e) * *(short *)(unaff_A6 + 0x72) * 2 +
          (ushort)*(byte *)(unaff_A6 + 0x83);
  if ((int)((uint)uVar1 << 0x10) < 0) {
    uVar1 = 0;
  }
  if (0x80 < (short)uVar1) {
    uVar1 = 0x80;
  }
  *(char *)(unaff_A6 + 0x83) = (char)uVar1;
  uVar2 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x30) = uVar2;
  func_0x000143e4();
  candidate_6d0f6();
  return;
}
