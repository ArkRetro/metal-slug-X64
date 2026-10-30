/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d390.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d390(void)

{
  uint uVar1;
  short sVar2;
  short sVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  if ((*(byte *)((int)unaff_A6 + 0x93) & 4) == 0) {
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    candidate_4256();
    *unaff_A6 = 0x5d570;
    uVar5 = 0;
    candidate_5e234();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_5d596;
      uVar5 = 0;
    }
    candidate_5e244();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_5d27a;
    }
  }
  else {
    unaff_A6[0xd] = 0xccccc;
    *(undefined4 *)((int)unaff_A6 + 0x76) = 0xfffae148;
    unaff_A6[0xc] = 0xfff9999a;
    candidate_4256();
    *unaff_A6 = 0x5d3c8;
    uVar5 = 0;
    func_0x000143e4();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      uVar1 = unaff_A6[0xd];
      uVar5 = CARRY4(*(uint *)((int)unaff_A6 + 0x76),uVar1);
      unaff_A6[0xd] = *(uint *)((int)unaff_A6 + 0x76) + uVar1;
    }
    else {
      candidate_1b1dc();
    }
    candidate_427a();
    if (((bool)uVar5) && (*(short *)(unaff_A6 + 0xc) == 0)) {
      *unaff_A6 = candidate_5d406;
    }
  }
  candidate_5e6d2();
  sVar2 = *(short *)(unaff_A6 + 0x1f);
  sVar3 = sVar2 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar3;
  if (-1 >= sVar3) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar4 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar4 = bVar4 && (-1 < sVar3 && sVar2 == 0);
  candidate_5e7a8();
  if (bVar4) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
