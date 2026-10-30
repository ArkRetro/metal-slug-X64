/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079848.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79848(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x7985e;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -8;
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x8a) == '\x01') {
    *unaff_A6 = 0x797fe;
  }
  if (*(char *)(unaff_A6[3] + 0x8a) == '\0') {
    *unaff_A6 = candidate_798a2;
  }
  uVar1 = false;
  candidate_4c24();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_5db92;
    uVar1 = 0;
  }
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
