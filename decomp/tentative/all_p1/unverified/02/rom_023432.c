/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x023432.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_23432(void)

{
  int iVar1;
  undefined4 uVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_120f8();
  candidate_4256();
  unaff_A6[0x21] = 0x2266a;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0;
  *(undefined4 *)(unaff_A5 + 0x694e) = 0xfffe0000;
  *(undefined4 *)(unaff_A5 + 0x6952) = 0xffffe000;
  *(undefined2 *)(unaff_A6 + 0x26) = 1;
  candidate_c8022();
  *unaff_A6 = 0x2348a;
  candidate_1abc8();
  candidate_234de();
  *(int *)(unaff_A5 + 0x694e) = -*(int *)(unaff_A5 + 0x694e);
  iVar1 = unaff_A6[3];
  *(undefined2 *)(iVar1 + 0x9a) = *(undefined2 *)((int)unaff_A6 + 0x9a);
  sVar3 = *(short *)(iVar1 + 0x9a) * 4;
  if (*(short *)((int)unaff_A6 + 0x9a) == 9) {
    sVar3 = sVar3 + -2;
  }
  uVar2 = unaff_A6[0xf];
  *(short *)(unaff_A6 + 0xf) = sVar3 + *(short *)(unaff_A6 + 0xf);
  candidate_427a();
  unaff_A6[0xf] = uVar2;
  if (*(short *)(unaff_A6 + 0x10) < 0x90) {
    *(undefined2 *)(unaff_A6 + 0x10) = 0x90;
    *(undefined4 *)(unaff_A5 + 0x6952) = 0;
  }
  return;
}
