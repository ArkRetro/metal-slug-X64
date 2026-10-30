/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c0d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c0d2(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x21) = 2;
  *unaff_A6 = 0x4c0ee;
  uVar3 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar3) {
    if (*(short *)(unaff_A6 + 0x21) == 0) {
      *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + 1;
      if (*(short *)((int)unaff_A6 + 0x86) < *(short *)((int)unaff_A6 + 0x82)) {
        if (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0xa4) == '\0') {
          *(undefined1 *)(unaff_A6 + 0x29) = 0;
          uVar2 = candidate_260c8();
          *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x29) = 1;
          uVar2 = candidate_260c8();
          *(undefined2 *)(unaff_A6 + 0x20) = uVar2;
        }
        *unaff_A6 = 0x4c060;
      }
      else {
        *unaff_A6 = candidate_4be04;
      }
      uVar3 = 0;
    }
    else {
      candidate_4256();
      sVar1 = *(short *)(unaff_A6 + 0x21);
      uVar3 = sVar1 == 0;
      *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
    }
  }
  candidate_4d74a();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = candidate_4be04;
  }
  return;
}
