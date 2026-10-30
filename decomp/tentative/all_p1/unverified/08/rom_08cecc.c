/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08cecc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8cecc(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short in_D0w;
  undefined4 *in_A0;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined1 uStack_5;

  if (in_D0w != 3) {
    in_D0w = in_D0w + 1;
  }
  uVar1 = *in_A0;
  candidate_57948();
  candidate_120f8();
  candidate_411c();
  *(undefined4 *)(extraout_A0 + 0x6e) = uVar1;
  *(byte *)(extraout_A0 + 0x8a) = (char)in_D0w - 1U & 1;
  if (uStack_5 == '\0') {
    iVar2 = *(int *)(unaff_A6 + 0x44);
    iVar3 = *(int *)(unaff_A6 + 0x48);
    iVar6 = *(int *)(unaff_A6 + 0x3c) - *(int *)(unaff_A5 + 0x697c);
    iVar7 = *(int *)(unaff_A6 + 0x40) - *(int *)(unaff_A5 + 0x6980);
    iVar4 = *(int *)(unaff_A5 + 0x6984);
    iVar5 = *(int *)(unaff_A5 + 0x6988);
    *(int *)(extraout_A0 + 0x28) = iVar6;
    *(int *)(extraout_A0 + 0x2c) = iVar7;
    *(int *)(extraout_A0 + 0x30) = -((iVar2 - iVar4) - iVar6);
    *(int *)(extraout_A0 + 0x34) = -((iVar3 - iVar5) - iVar7);
    *(int *)(extraout_A0 + 0x38) = unaff_A5 + 0x6968;
    candidate_1abc8();
    return;
  }
  candidate_1ab98();
  return;
}
