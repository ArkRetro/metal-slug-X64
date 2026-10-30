/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x028052.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_28052(void)

{
  undefined4 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  if ((*(byte *)((int)unaff_A6 + 0x7d) & 0x20) == 0) {
    uVar2 = 0;
    candidate_120f8();
    candidate_1b1dc();
    candidate_28d24();
    if ((bool)uVar2) {
      candidate_4256();
      uVar1 = 0x10000;
    }
    else {
      candidate_4256();
      uVar1 = 0xffff0000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar1;
    *(undefined2 *)(unaff_A6 + 0x1e) = 0;
    candidate_28d04();
    *unaff_A6 = 0x280b6;
    candidate_146a8();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *unaff_A6 = candidate_2829e;
    }
    if (unaff_A6[0xc] == 0) {
      *unaff_A6 = candidate_2829e;
    }
    uVar2 = 0;
    candidate_427a();
    candidate_28d52();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2829e;
      uVar2 = 0;
    }
    candidate_28da8();
    if ((bool)uVar2) {
      *unaff_A6 = 0x282a4;
      uVar2 = 0;
    }
    candidate_28dea();
    if ((bool)uVar2) {
      *unaff_A6 = 0x282a4;
      uVar2 = 0;
    }
    candidate_49e8();
  }
  else {
    candidate_1b1dc();
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
    candidate_4256();
    *unaff_A6 = 0x2813a;
    uVar2 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar2) {
      if ((*(byte *)((int)unaff_A6 + 0x7d) & 0x20) == 0) {
        *unaff_A6 = candidate_2820a;
        uVar2 = 0;
      }
      else {
        uVar2 = 0;
        candidate_4256();
      }
    }
    candidate_28da8();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2834a;
      uVar2 = 0;
    }
    candidate_28dea();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2818e;
      uVar2 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    candidate_1b206();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2849a;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_284f2;
    bVar3 = false;
    candidate_1b206();
    if (bVar3) {
      *unaff_A6 = candidate_2858c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
