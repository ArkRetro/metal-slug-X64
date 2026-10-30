/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a3352.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a3352(void)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar4;
  bool bVar5;

  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x40) == 0) {
    candidate_a3fc0();
    if ((bool)in_CF) {
      candidate_1b1dc();
      if (*(char *)((int)unaff_A6 + 0x8b) == '\0') {
        *(undefined2 *)(unaff_A6 + 0x21) = 0x32;
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x10;
        if (*(int *)((int)unaff_A6 + 0x76) != 0) {
          candidate_a3ef0();
        }
        candidate_a3ef0();
        candidate_4256();
        *unaff_A6 = 0xa341e;
        candidate_a3f52();
        candidate_427a();
        sVar1 = *(short *)(unaff_A6 + 0x21);
        sVar2 = sVar1 + -1;
        *(short *)(unaff_A6 + 0x21) = sVar2;
        if (sVar2 == 0) {
          *unaff_A6 = candidate_a343a;
        }
        uVar4 = sVar2 != 0 && sVar1 == 0;
      }
      else {
        uVar3 = candidate_260c8();
        *(undefined2 *)(unaff_A6 + 0x22) = uVar3;
        if ((*(byte *)((int)unaff_A6 + 0x96) & 0x10) == 0) {
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
        *unaff_A6 = 0xa34de;
        uVar4 = 0;
        candidate_a3f52();
        candidate_427a();
        if (((bool)uVar4) && (*(char *)((int)unaff_A6 + 0x8b) < '\x01')) {
          *unaff_A6 = candidate_a352c;
        }
        if ((('\0' < *(char *)((int)unaff_A6 + 0x8b)) &&
            (*(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1,
            *(short *)(unaff_A6 + 0x22) < 1)) &&
           (*(char *)((int)unaff_A6 + 0x8b) = *(char *)((int)unaff_A6 + 0x8b) + -1,
           '\0' < *(char *)((int)unaff_A6 + 0x8b))) {
          *unaff_A6 = 0xa344a;
        }
        uVar4 = false;
      }
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        *unaff_A6 = candidate_a30b2;
        uVar4 = false;
      }
      candidate_4c24();
      if ((bool)uVar4) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        uVar4 = 0;
      }
      goto LAB_000a37f6;
    }
    if (*(int *)((int)unaff_A6 + 0x76) != 0) {
      candidate_a3ef0();
    }
    candidate_a3ef0();
    candidate_4256();
    *unaff_A6 = 0xa33a2;
    uVar4 = 0;
    candidate_a3f52();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_a3116;
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
    bVar5 = false;
    candidate_1b206();
    if (bVar5) {
      *unaff_A6 = candidate_a3624;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
LAB_000a37f6:
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
