/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d4c26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d4c26(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  *(undefined2 *)(unaff_A6 + 0x23) = 1;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xd4c5e;
  uVar1 = candidate_d5090();
  *(undefined2 *)(unaff_A6 + 10) = uVar1;
  uVar3 = 0;
  candidate_d54ee();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_d4cc2;
  }
  sVar2 = (short)*(int *)((int)unaff_A6 + 0x9a);
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    sVar2 = candidate_49e8();
  }
  if (sVar2 != 0) {
    *unaff_A6 = 0xd4fde;
  }
  uVar3 = false;
  candidate_409a();
  if (!(bool)uVar3) {
    *unaff_A6 = 0xd4fde;
    uVar3 = 0;
  }
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
