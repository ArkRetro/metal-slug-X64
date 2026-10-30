/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f9fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f9fa(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) ^ 1;
  if ((*(byte *)(unaff_A6 + 0x24) & 0x20) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 5;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 6;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  }
  *unaff_A6 = 0x9fa28;
  candidate_a0576();
  candidate_a0356();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    *unaff_A6 = candidate_9f846;
  }
  if (((*(byte *)(unaff_A6 + 0x24) & 0x80) == 0) &&
     (*(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1,
     *(short *)(unaff_A6 + 0x20) < 1)) {
    *unaff_A6 = candidate_9fc9c;
  }
  uVar1 = false;
  candidate_a072a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9fade;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x9fca6;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
