/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d26ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d26ec(void)

{
  int iVar1;
  short sVar2;
  undefined2 uVar3;
  int *unaff_A6;
  undefined1 uVar4;

  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xfe;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xfd;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) & 0xf7;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar3;
  candidate_d34ce();
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0;
  candidate_4256();
  *unaff_A6 = 0xd2460;
  candidate_d3220();
  candidate_427a();
  sVar2 = candidate_d3436();
  iVar1 = *(int *)((short)(sVar2 << 2) + 0xe8a04);
  if (iVar1 != 0) {
    *unaff_A6 = iVar1;
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  uVar4 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 8;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  candidate_d341e();
  if ((bool)uVar4) {
    *unaff_A6 = (int)candidate_d2730;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = (int)candidate_d2730;
  }
  return;
}
