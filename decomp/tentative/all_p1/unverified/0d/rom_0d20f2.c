/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d20f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_d20f2(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  candidate_9ea8();
  candidate_4520();
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined2 *)(unaff_A6 + 0x1f) = 0x38;
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 3;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar1;
  candidate_4256();
  *unaff_A6 = 0xd2164;
  uVar3 = 0;
  candidate_d3220();
  candidate_d3268();
  candidate_427a();
  sVar2 = candidate_d3462();
  if (((bool)uVar3) && ((_DAT_0010000e & 7) == 0)) {
    *(short *)((int)unaff_A6 + 0x8e) = sVar2 + *(short *)((int)unaff_A6 + 0x8e);
  }
  if ((*(byte *)(unaff_A6[3] + 0x70) & 4) != 0) {
    *unaff_A6 = candidate_d21a4;
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  bVar4 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x10;
  }
  candidate_d341e();
  if (bVar4) {
    *unaff_A6 = candidate_d231a;
  }
  return;
}
