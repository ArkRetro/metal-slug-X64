/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3116.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3116(void)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  candidate_1b1dc();
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x40) == 0) {
    candidate_120f8();
    candidate_1b1dc();
    uVar2 = 0x30000;
    if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0) {
      uVar2 = 0xfffd0000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
    bVar3 = false;
    candidate_a3f6c();
    if (bVar3) {
      if (*(int *)((int)unaff_A6 + 0x76) != 0) {
        candidate_a3ef0();
      }
      candidate_a3ef0();
      candidate_4256();
    }
    else {
      if (*(int *)((int)unaff_A6 + 0x76) != 0) {
        candidate_a3ef0();
      }
      candidate_a3ef0();
      candidate_4256();
    }
    candidate_a3fe2();
    *unaff_A6 = 0xa31d6;
    candidate_146a8();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *unaff_A6 = 0xa32ae;
    }
    if (unaff_A6[0xc] == 0) {
      *unaff_A6 = 0xa32ae;
    }
    uVar4 = 0;
    candidate_427a();
    candidate_a3fa0();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_a32a8;
      uVar4 = 0;
    }
    candidate_a3fc0();
    if ((bool)uVar4) {
      *unaff_A6 = 0xa32ae;
      uVar4 = 0;
    }
    candidate_49e8();
  }
  else {
    candidate_1b1dc();
    *(undefined2 *)(unaff_A6 + 0x21) = 0xf;
    if (*(int *)((int)unaff_A6 + 0x76) != 0) {
      candidate_a3ef0();
    }
    candidate_a3ef0();
    candidate_4256();
    *unaff_A6 = 0xa3274;
    uVar4 = 0;
    candidate_a3f52();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x96) & 0x40) == 0) {
      sVar1 = *(short *)(unaff_A6 + 0x21);
      uVar4 = sVar1 == 0;
      sVar1 = sVar1 + -1;
      *(short *)(unaff_A6 + 0x21) = sVar1;
      if (sVar1 == 0) {
        *unaff_A6 = candidate_a3352;
        uVar4 = 0;
      }
    }
    candidate_a3fc0();
    if ((bool)uVar4) {
      *unaff_A6 = 0xa33c4;
      uVar4 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_a30b2;
    uVar4 = 0;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_a35e8;
    bVar3 = false;
    candidate_1b206();
    if (bVar3) {
      *unaff_A6 = candidate_a3624;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a36ac;
    uVar4 = false;
    candidate_1b206();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_a36f4;
      uVar4 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_a378a;
      uVar4 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = 0xa385c;
  }
  return;
}
