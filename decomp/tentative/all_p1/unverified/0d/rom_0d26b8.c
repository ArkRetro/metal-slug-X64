/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d26b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d26b8(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  candidate_4256();
  *unaff_A6 = 0xd26d4;
  uVar1 = 0;
  candidate_d3220();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d26ec;
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  uVar1 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 8;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_d341e();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d2730;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d2730;
  }
  return;
}
