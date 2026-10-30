/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07bcbc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7bcbc(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar2;
  bool bVar3;

  candidate_7c0ac();
  if ((bool)in_CF) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar1;
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
    bVar3 = false;
    if ((*(byte *)((int)unaff_A6 + 0x81) & 1) == 0) {
      candidate_7c0ea();
      if (bVar3) {
        candidate_4256();
      }
      else {
        candidate_4256();
      }
    }
    else {
      candidate_7c0ea();
      if (bVar3) {
        candidate_4256();
      }
      else {
        candidate_4256();
      }
    }
    *unaff_A6 = 0x7bde0;
    uVar2 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = 0x7bd64;
      *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + 1;
      *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
      uVar2 = 0;
      if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
        *unaff_A6 = candidate_7bc10;
        uVar1 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
        uVar2 = 0;
      }
    }
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x81) & 1) == 0) {
      candidate_7c0ea();
      if ((bool)in_CF) {
        candidate_4256();
      }
      else {
        candidate_4256();
      }
    }
    else {
      candidate_7c0ea();
      if ((bool)in_CF) {
        candidate_4256();
      }
      else {
        candidate_4256();
      }
    }
    *(byte *)((int)unaff_A6 + 0x81) = *(byte *)((int)unaff_A6 + 0x81) ^ 1;
    *unaff_A6 = 0x7bd36;
    uVar2 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = 0x7bd50;
      uVar2 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7be22;
      uVar2 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_7be86;
  }
  return;
}
