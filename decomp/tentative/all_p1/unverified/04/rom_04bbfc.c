/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04bbfc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4bbfc(void)

{
  ushort uVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0xa4) == '\0') {
    *(undefined1 *)(unaff_A6 + 0x29) = 0;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x29) = 1;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
  }
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *unaff_A6 = 0x4bc68;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x21) < *(short *)(unaff_A6 + 0x20)) {
    uVar1 = *(ushort *)(unaff_A6 + 0x21);
    uVar3 = 0xfffe < uVar1;
    *(ushort *)(unaff_A6 + 0x21) = uVar1 + 1;
  }
  else {
    *unaff_A6 = candidate_4bca2;
    uVar3 = false;
  }
  candidate_4d9de();
  candidate_4d74a();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = candidate_4be04;
  }
  return;
}
