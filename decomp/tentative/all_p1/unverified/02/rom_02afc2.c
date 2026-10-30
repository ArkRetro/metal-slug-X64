/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02afc2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2afc2(void)

{
  int iVar1;
  short sVar2;
  int extraout_A0;
  int extraout_A0_00;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_120f8();
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_00;
  }
  iVar1 = *(int *)((int)unaff_A6 + 0x62);
  *(int *)(iVar3 + 0x30) = iVar1 >> 1;
  unaff_A6[0xd] = 0xaaaaa;
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xfffed098;
  unaff_A6[0xc] = 0;
  unaff_A6[0xc] = iVar1 >> 2;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x20;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *unaff_A6 = 0x2b050;
  bVar4 = false;
  candidate_2bdd8();
  if (bVar4) {
    *unaff_A6 = candidate_2b0bc;
  }
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x121) {
    *unaff_A6 = candidate_2b0bc;
  }
  sVar2 = candidate_49e8();
  if (sVar2 != 0) {
    *unaff_A6 = candidate_2b0e2;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2b0e2;
  }
  bVar4 = false;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
