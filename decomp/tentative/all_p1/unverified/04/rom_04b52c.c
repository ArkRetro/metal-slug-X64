/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b52c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b52c(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

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
  if (*(short *)((int)unaff_A6 + 0x82) == 0) {
    *(undefined2 *)(unaff_A6 + 0x22) = 4;
    *(undefined1 *)(unaff_A6 + 0x27) = 0;
    candidate_4256();
    *unaff_A6 = 0x4b752;
    candidate_1abc8();
    if (*(short *)(unaff_A6 + 0x22) == 0) {
      *(undefined2 *)(unaff_A6 + 0x22) = 4;
      uVar4 = false;
      candidate_4d768();
      if ((bool)uVar4) {
        *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0x9e) = 0;
        *unaff_A6 = candidate_4b40c;
        uVar4 = 0;
      }
    }
    else {
      sVar1 = *(short *)(unaff_A6 + 0x22);
      uVar4 = sVar1 == 0;
      *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
    }
    candidate_4d9de();
    candidate_4d74a();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    }
    return;
  }
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x22) = 3;
  *(undefined2 *)(unaff_A6 + 0x21) = *(undefined2 *)(unaff_A6 + 0x20);
  candidate_4d798();
  *unaff_A6 = 0x4b5bc;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    *(undefined2 *)(unaff_A6 + 0x22) = 3;
    bVar3 = false;
    candidate_4d768();
    if (bVar3) {
      if (*(short *)(unaff_A6 + 0x21) == 0) {
        *unaff_A6 = candidate_4b658;
      }
      else {
        candidate_4d798();
      }
    }
  }
  else {
    *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
  }
  if (*(short *)(unaff_A6 + 0x21) != 0) {
    *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
  }
  *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6[3] + 0x98);
  if (*(short *)(unaff_A6[3] + 0x98) < 0x25) {
    uVar4 = 0;
    candidate_4256();
    candidate_4d9de();
  }
  else {
    uVar4 = 0;
    candidate_4256();
    candidate_4d9de();
  }
  candidate_4d74a();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
    *unaff_A6 = 0x4b732;
  }
  return;
}
