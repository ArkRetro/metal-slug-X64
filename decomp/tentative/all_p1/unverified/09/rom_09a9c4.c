/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a9c4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a9c4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_9ea8();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  *(undefined2 *)((int)unaff_A6 + 0x12) = *(undefined2 *)(unaff_A6[3] + 0x12);
  candidate_1ad6e();
  *unaff_A6 = 0x9a9fa;
  candidate_1abc8();
  candidate_11ff6();
  if (*(char *)(unaff_A6[3] + 0x5a) != '\0') {
    *unaff_A6 = candidate_9aa48;
  }
  uVar1 = false;
  candidate_409a();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_9a5ee;
    uVar1 = 0;
  }
  candidate_c8bec();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
