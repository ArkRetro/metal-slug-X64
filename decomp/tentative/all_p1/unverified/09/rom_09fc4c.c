/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fc4c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fc4c(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 4;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0x9fc6c;
  candidate_a0334();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
    if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
      *unaff_A6 = candidate_9f846;
    }
    else {
      *unaff_A6 = candidate_9fa48;
    }
  }
  if (((*(byte *)(unaff_A6 + 0x24) & 0x80) == 0) &&
     (*(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1,
     *(short *)(unaff_A6 + 0x20) < 1)) {
    *unaff_A6 = candidate_9fc9c;
  }
  uVar2 = false;
  candidate_a072a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9fade;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x9fca6;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
