/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d28ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d28ae(void)

{
  int iVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9ea8();
  iVar1 = *(int *)((short)(*(short *)((int)unaff_A6 + 0x8e) << 2) + 0xe8660);
  sVar3 = *(short *)((int)unaff_A6 + 0x92) << 2;
  uVar2 = *(undefined2 *)(iVar1 + 2 + (int)sVar3);
  *(undefined2 *)((int)unaff_A6 + 0x7a) = *(undefined2 *)(iVar1 + sVar3);
  *(undefined2 *)(unaff_A6 + 0x1f) = uVar2;
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0xd2912;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
    uVar4 = 0;
  }
  candidate_19504();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
