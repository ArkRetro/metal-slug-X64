/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04bf80.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4bf80(void)

{
  ushort uVar1;
  undefined2 uVar2;
  char cVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)(unaff_A6 + 0x22) = 4;
  if (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0xa4) == '\0') {
    *(undefined1 *)(unaff_A6 + 0x29) = 0;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
    cVar3 = candidate_aaa88();
    if (cVar3 == '\0') {
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
    }
    else {
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x29) = 1;
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
    cVar3 = candidate_aaa88();
    if (cVar3 == '\0') {
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
    }
    else {
      uVar2 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
    }
  }
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *unaff_A6 = 0x4c07a;
  candidate_1abc8();
  candidate_4d94a();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    candidate_4d768();
    *(undefined2 *)(unaff_A6 + 0x22) = 4;
  }
  else {
    *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
  }
  if (*(short *)(unaff_A6 + 0x21) < *(short *)(unaff_A6 + 0x20)) {
    uVar1 = *(ushort *)(unaff_A6 + 0x21);
    uVar4 = 0xfffe < uVar1;
    *(ushort *)(unaff_A6 + 0x21) = uVar1 + 1;
  }
  else {
    *unaff_A6 = candidate_4c0d2;
    uVar4 = false;
  }
  candidate_4d9de();
  candidate_4d74a();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = candidate_4be04;
  }
  return;
}
