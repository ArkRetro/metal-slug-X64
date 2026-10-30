/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a12c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a12c0(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_13f1e();
  candidate_18f92();
  candidate_120f8();
  candidate_12192();
  candidate_12192();
  candidate_12192();
  candidate_12192();
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(undefined1 *)(iVar1 + 0x60) = *(undefined1 *)(unaff_A6 + 0x18);
  *(undefined1 *)(unaff_A6 + 0x18) = 0xfd;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  candidate_a2ae8();
  candidate_a2958();
  candidate_1da40();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x78;
  unaff_A6[0xd] = 0xfffff000;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_4256();
  candidate_9bb0();
  *unaff_A6 = candidate_a13ac;
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
    candidate_120f8();
    candidate_c8022();
    unaff_A6[0xd] = 0xffff0000;
    *unaff_A6 = 0xa13f8;
  }
  candidate_1abc8();
  candidate_1ac38();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x71) {
    *(undefined1 *)((int)unaff_A6 + 0x91) = 1;
    *unaff_A6 = candidate_a142a;
  }
  candidate_a2680();
  return;
}
