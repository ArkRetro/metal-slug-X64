/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6464.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6464(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_1ad6e();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(int *)((int)unaff_A6 + 0x8e) = iVar1;
  *(undefined2 *)(iVar1 + 0x80) = 0xffff;
  *(undefined4 *)((int)unaff_A6 + 0x92) = 0xe9218;
  candidate_9ea8();
  candidate_4520();
  candidate_4256();
  *unaff_A6 = 0xd64d8;
  uVar3 = 0;
  candidate_d7126();
  candidate_44f6();
  candidate_1abc8();
  candidate_427a();
  candidate_409a();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
