/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029efc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29efc(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar1;
  candidate_4256();
  *unaff_A6 = 0x29f22;
  candidate_2a472();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  uVar2 = 0;
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    if (*(char *)(unaff_A6 + 0x21) == '\0') {
      *unaff_A6 = candidate_29f8c;
    }
    if (*(byte *)(unaff_A6 + 0x21) == 2) {
      *unaff_A6 = candidate_29f8c;
    }
    uVar2 = *(byte *)(unaff_A6 + 0x21) < 2;
    candidate_1affc();
    if (!(bool)uVar2) {
      *unaff_A6 = candidate_29f8c;
      uVar2 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 8;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2a04c;
    uVar2 = 0;
  }
  if (((*(byte *)((int)unaff_A6 + 0x86) & 0x80) != 0) &&
     ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
    *unaff_A6 = candidate_2a0b2;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
