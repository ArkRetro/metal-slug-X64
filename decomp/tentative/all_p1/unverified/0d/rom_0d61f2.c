/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d61f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d61f2(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 extraout_A0_01;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4256();
  bVar2 = false;
  candidate_40bc();
  iVar1 = extraout_A0;
  if (bVar2) {
    candidate_1b220();
    iVar1 = extraout_A0_00;
  }
  *(int *)((int)unaff_A6 + 0x8e) = iVar1;
  *(undefined2 *)(iVar1 + 0x80) = 0xffff;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0xd6254;
  candidate_1af9a();
  *(undefined4 *)((int)unaff_A6 + 0x8a) = extraout_A0_01;
  *(short *)(unaff_A6 + 0xb) = (short)*(char *)(unaff_A6[3] + 0xa5);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x20) + *(short *)(unaff_A6[3] + 0x1e);
  candidate_1abc8();
  candidate_427a();
  bVar2 = *(byte *)(*(int *)((int)unaff_A6 + 0x8e) + 0xa6) < 0xd;
  if (!bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
    *unaff_A6 = candidate_d62a2;
  }
  candidate_19424();
  if (bVar2) {
    *unaff_A6 = candidate_d63ae;
  }
  return;
}
