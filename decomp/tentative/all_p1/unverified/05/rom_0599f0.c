/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0599f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_599f0(void)

{
  undefined4 uVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A6;

  candidate_40bc();
  candidate_1b220();
  iVar2 = *(int *)(unaff_A6 + 0x34);
  *(int *)(extraout_A0 + 0x30) = *(int *)(unaff_A6 + 0x30) >> 1;
  *(int *)(extraout_A0 + 0x34) = iVar2 >> 1;
  uVar1 = 0xc0000;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    uVar1 = 0xfff40000;
  }
  *(undefined4 *)(extraout_A0 + 0x6e) = uVar1;
  *(undefined4 *)(extraout_A0 + 0x72) = 0xfffee000;
  return;
}
