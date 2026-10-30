/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fa48.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fa48(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
    if ((*(byte *)(unaff_A6 + 0x24) & 0x20) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 9;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 10;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    }
    candidate_4256();
  }
  else {
    if ((*(byte *)(unaff_A6 + 0x24) & 0x20) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 7;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 8;
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
    }
    candidate_4256();
  }
  *unaff_A6 = 0x9fac0;
  uVar1 = 0;
  candidate_a0334();
  candidate_427a();
  if ((bool)uVar1) {
    *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) ^ 2;
    *unaff_A6 = candidate_9f846;
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
