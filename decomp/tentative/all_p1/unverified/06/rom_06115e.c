/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06115e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6115e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + 1;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x611c2;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
