/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d464.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d464(void)

{
  short sVar1;
  undefined1 uVar3;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  bool bVar4;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  uVar3 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x83) = uVar3;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1f) = uVar2;
  unaff_A6[0x14] = 0x5e948;
  candidate_4256();
  *unaff_A6 = 0x5d4aa;
  uVar3 = 0;
  candidate_5e234();
  candidate_427a();
  if (((bool)uVar3) && (uVar3 = 0, *(short *)(unaff_A6 + 0x1f) < 1)) {
    *(char *)((int)unaff_A6 + 0x83) = *(char *)((int)unaff_A6 + 0x83) + -1;
    if (*(char *)((int)unaff_A6 + 0x83) < '\x01') {
      *unaff_A6 = candidate_5d406;
      uVar2 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x1f) = uVar2;
      uVar3 = 0;
    }
    else {
      *unaff_A6 = 0x5d47c;
      uVar3 = 0;
    }
  }
  candidate_5e244();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_5d27a;
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f) + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar1;
  if (sVar1 < 0) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db92;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
