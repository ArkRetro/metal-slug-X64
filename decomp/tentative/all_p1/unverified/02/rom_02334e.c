/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02334e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2334e(void)

{
  int iVar1;
  undefined4 uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_4256();
  unaff_A6[0x21] = 0x2245e;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0;
  candidate_25bc2();
  *unaff_A6 = 0x23378;
  uVar4 = 0;
  candidate_1abc8();
  candidate_234de();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_233be;
  }
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
  return;
}
