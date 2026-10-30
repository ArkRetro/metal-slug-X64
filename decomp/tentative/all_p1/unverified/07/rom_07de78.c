/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07de78.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7de78(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  if (*(short *)(unaff_A6 + 0x1d) != -1) {
    *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  }
  bVar2 = false;
  if ((*(short *)((int)unaff_A6 + 0x72) == 0) || (candidate_7e24c(), bVar2)) {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1f) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar1;
    *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)((int)unaff_A6 + 0x7a);
    candidate_4256();
    *unaff_A6 = 0x7ded2;
    uVar3 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar3) {
      uVar3 = 0;
      candidate_4256();
    }
    candidate_7e290();
    if ((bool)uVar3) {
      *unaff_A6 = 0x7deb6;
      *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + -1;
      if (*(short *)(unaff_A6 + 0x1f) < 1) {
        *unaff_A6 = candidate_7de3c;
        uVar1 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
      }
    }
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1f) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar1;
    *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)((int)unaff_A6 + 0x7a);
    candidate_4256();
    *unaff_A6 = 0x7df64;
    uVar3 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar3) {
      uVar3 = 0;
      candidate_4256();
    }
    candidate_7e290();
    if ((bool)uVar3) {
      *unaff_A6 = 0x7df48;
      *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + -1;
      if (*(short *)(unaff_A6 + 0x1f) < 1) {
        *unaff_A6 = candidate_7de3c;
        uVar1 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
      }
    }
  }
  if (*(char *)(unaff_A6[3] + 0x60) != '\0') {
    *unaff_A6 = candidate_7dfba;
  }
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    *unaff_A6 = candidate_7dfba;
  }
  bVar2 = false;
  candidate_4c24();
  if (bVar2) {
    *unaff_A6 = candidate_7dfea;
  }
  *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
  if (*(short *)(unaff_A6 + 0xf) < 1) {
    *unaff_A6 = candidate_7e09e;
  }
  return;
}
