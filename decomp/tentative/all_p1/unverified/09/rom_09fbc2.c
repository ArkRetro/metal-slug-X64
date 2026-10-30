/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fbc2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fbc2(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;

  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x87) = uVar2;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
  if ((*(byte *)((int)unaff_A6 + 0x87) & 1) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 2;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 3;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  }
  *unaff_A6 = 0x9fc0a;
  candidate_a0334();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x87) < '\x02') {
    if (*(char *)((int)unaff_A6 + 0x61) != '\0') {
      *unaff_A6 = candidate_9fc4c;
    }
    uVar2 = false;
  }
  else {
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    uVar2 = *(short *)((int)unaff_A6 + 0x7e) == 0;
    if (*(short *)((int)unaff_A6 + 0x7e) < 2) {
      *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + -1;
      *unaff_A6 = 0x9fbd2;
      uVar2 = false;
    }
  }
  if ((*(byte *)(unaff_A6 + 0x24) & 0x80) == 0) {
    *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
    uVar2 = false;
    if (*(short *)(unaff_A6 + 0x20) < 1) {
      *unaff_A6 = candidate_9fc9c;
      uVar2 = false;
    }
  }
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
