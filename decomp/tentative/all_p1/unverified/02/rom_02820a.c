/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02820a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2820a(void)

{
  uint uVar1;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  bVar3 = false;
  candidate_28da8();
  if (bVar3) {
    candidate_1b1dc();
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
    candidate_4256();
    *unaff_A6 = 0x2836c;
    uVar4 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_28386;
      uVar4 = 0;
    }
    candidate_4c24();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x7d) & 0x20) == 0) {
      candidate_28d24();
      if (bVar3) {
        candidate_4256();
        iVar2 = 0x10000;
      }
      else {
        candidate_4256();
        iVar2 = -0x10000;
      }
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        iVar2 = -iVar2;
      }
      *(int *)((int)unaff_A6 + 0x72) = iVar2 >> 5;
      *unaff_A6 = 0x28272;
      candidate_146a8();
      uVar1 = *(uint *)((int)unaff_A6 + 0x62);
      uVar4 = CARRY4(*(uint *)((int)unaff_A6 + 0x72),uVar1);
      *(uint *)((int)unaff_A6 + 0x62) = *(uint *)((int)unaff_A6 + 0x72) + uVar1;
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_28052;
        uVar4 = 0;
      }
      candidate_49e8();
    }
    else {
      candidate_1b1dc();
      *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
      candidate_4256();
      *unaff_A6 = 0x2813a;
      uVar4 = 0;
      candidate_1436a();
      candidate_427a();
      if ((bool)uVar4) {
        if ((*(byte *)((int)unaff_A6 + 0x7d) & 0x20) == 0) {
          *unaff_A6 = candidate_2820a;
          uVar4 = 0;
        }
        else {
          uVar4 = 0;
          candidate_4256();
        }
      }
      candidate_28da8();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_2834a;
        uVar4 = 0;
      }
      candidate_28dea();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_2818e;
        uVar4 = 0;
      }
    }
    candidate_4c24();
    if ((bool)uVar4) {
      candidate_1b206();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_2849a;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
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
