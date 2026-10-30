/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x014742.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_14742(void)

{
  undefined2 uVar1;
  short in_D0w;
  int iVar2;
  int unaff_D6;
  int unaff_A5;
  int unaff_A6;
  bool bVar3;
  short sStack_4;
  short sStack_2;

  sStack_2 = -0x8000;
  sStack_4 = 4;
  iVar2 = (uint)(ushort)-in_D0w << 0x10;
  uVar1 = *(undefined2 *)(unaff_A6 + 0x28);
  candidate_128e8();
  *(undefined1 *)(unaff_A5 + 0x4008) = 0xff;
  while (candidate_1abc8(), -1 < (short)(*(short *)(unaff_A6 + 0x3c) + -0x1c)) {
    bVar3 = false;
    candidate_13db2();
    if (bVar3) {
      if ((iVar2 < unaff_D6) && (bVar3 = sStack_4 == 0, sStack_4 = sStack_4 + -1, bVar3)) {
        sStack_2 = *(short *)(unaff_A6 + 0x28) + 10;
        iVar2 = unaff_D6;
      }
    }
    else {
      sStack_4 = 4;
    }
    *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + -5;
  }
  sStack_4 = 4;
  *(undefined2 *)(unaff_A6 + 0x28) = uVar1;
  while (candidate_1abc8(), (short)(*(short *)(unaff_A6 + 0x3c) + -0x124) < 0) {
    bVar3 = false;
    candidate_13db2();
    if (bVar3) {
      if ((iVar2 < unaff_D6) && (bVar3 = sStack_4 == 0, sStack_4 = sStack_4 + -1, bVar3)) {
        sStack_2 = *(short *)(unaff_A6 + 0x28) + -10;
        iVar2 = unaff_D6;
      }
    }
    else {
      sStack_4 = 4;
    }
    *(short *)(unaff_A6 + 0x28) = *(short *)(unaff_A6 + 0x28) + 5;
  }
  if (sStack_2 == -0x8000) {
    *(undefined2 *)(unaff_A6 + 0x28) = uVar1;
    return;
  }
  *(short *)(unaff_A6 + 0x28) = sStack_2;
  return;
}
