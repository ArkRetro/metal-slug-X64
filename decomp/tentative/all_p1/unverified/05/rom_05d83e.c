/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d83e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d83e(void)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  ushort uVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  candidate_1b1dc();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar3;
  bVar5 = false;
  candidate_5e2e4();
  if ((bVar5) &&
     (uVar4 = candidate_29c6(), (uVar4 & 0xff) < (*(ushort *)((int)unaff_A6 + 0x7a) & 0xff))) {
    candidate_1b1dc();
    uVar6 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar6 = 8;
    }
    *(undefined1 *)((int)unaff_A6 + 0x89) = uVar6;
    candidate_5e800();
    candidate_4256();
    *unaff_A6 = 0x5d8fe;
    uVar6 = 0;
    candidate_5e234();
    candidate_427a();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_5d778;
    }
  }
  else {
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar3;
    candidate_4256();
    *unaff_A6 = 0x5d89a;
    candidate_5e234();
    candidate_427a();
    *(short *)((int)unaff_A6 + 0x7a) = *(short *)((int)unaff_A6 + 0x7a) + -1;
    if (*(short *)((int)unaff_A6 + 0x7a) < 1) {
      *unaff_A6 = candidate_5d778;
    }
    bVar5 = false;
    candidate_5e244();
    if (bVar5) {
      *unaff_A6 = candidate_5d778;
    }
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar5 = bVar5 && (-1 < sVar2 && sVar1 == 0);
  candidate_5e7a8();
  if (bVar5) {
    *unaff_A6 = candidate_5d732;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db18;
  }
  bVar5 = false;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
