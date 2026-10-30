/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074b9e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1436a(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int unaff_A5;
  int unaff_A6;

  candidate_128da();
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  uVar1 = *(undefined4 *)(unaff_A6 + 0x30);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x34);
  func_0x0001448c();
  *(undefined4 *)(unaff_A6 + 0x34) = uVar2;
  *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
  return;
}
