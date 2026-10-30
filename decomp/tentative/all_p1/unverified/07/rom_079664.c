/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079664.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79664(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x7967a;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -8;
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x8a) == '\x02') {
    *unaff_A6 = candidate_796be;
  }
  if (*(char *)(unaff_A6[3] + 0x8a) == '\0') {
    *unaff_A6 = candidate_79718;
  }
  uVar1 = false;
  candidate_4c24();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_5db18;
    uVar1 = 0;
  }
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
