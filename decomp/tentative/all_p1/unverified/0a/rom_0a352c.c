/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a352c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a352c(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar2;
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x10) == 0) {
    candidate_1b1dc();
    *(undefined2 *)(unaff_A6 + 0x21) = 0xf;
    if (*(int *)((int)unaff_A6 + 0x76) != 0) {
      candidate_a3ef0();
    }
    candidate_a3ef0();
    candidate_4256();
    *unaff_A6 = 0xa3274;
    uVar3 = 0;
    candidate_a3f52();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x96) & 0x40) == 0) {
      sVar1 = *(short *)(unaff_A6 + 0x21);
      uVar3 = sVar1 == 0;
      sVar1 = sVar1 + -1;
      *(short *)(unaff_A6 + 0x21) = sVar1;
      if (sVar1 == 0) {
        *unaff_A6 = candidate_a3352;
        uVar3 = 0;
      }
    }
    candidate_a3fc0();
    if ((bool)uVar3) {
      *unaff_A6 = 0xa33c4;
      uVar3 = 0;
    }
  }
  else {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xef;
    if (*(int *)((int)unaff_A6 + 0x76) != 0) {
      candidate_a3ef0();
    }
    candidate_a3ef0();
    candidate_4256();
    *unaff_A6 = 0xa3590;
    uVar3 = 0;
    candidate_a3f52();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = 0xa322a;
      uVar3 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_a30b2;
    uVar3 = 0;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_a35e8;
    bVar4 = false;
    candidate_1b206();
    if (bVar4) {
      *unaff_A6 = candidate_a3624;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a36ac;
    uVar3 = false;
    candidate_1b206();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_a36f4;
      uVar3 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_a378a;
      uVar3 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = 0xa385c;
  }
  return;
}
