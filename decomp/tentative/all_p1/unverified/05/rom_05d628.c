/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d628.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d628(void)

{
  short sVar1;
  short sVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_4256();
  *unaff_A6 = 0x5d63e;
  uVar5 = 0;
  candidate_5e234();
  candidate_427a();
  if (((bool)uVar5) && (uVar5 = 0, *(short *)(unaff_A6 + 0x1f) < 1)) {
    *(char *)((int)unaff_A6 + 0x83) = *(char *)((int)unaff_A6 + 0x83) + -1;
    if (*(char *)((int)unaff_A6 + 0x83) < '\x01') {
      *unaff_A6 = candidate_5d696;
      uVar3 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x1f) = uVar3;
      uVar5 = 0;
    }
    else {
      *unaff_A6 = candidate_5d628;
      uVar5 = 0;
    }
  }
  candidate_5e244();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_5d27a;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f);
  sVar2 = sVar1 + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar2;
  if (-1 >= sVar2) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar4 = (*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0;
  if (!bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0x1e;
  }
  bVar4 = bVar4 && (-1 < sVar2 && sVar1 == 0);
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
