/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d23d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d23d4(void)

{
  int iVar1;
  undefined2 uVar2;
  short sVar3;
  int *unaff_A6;
  undefined1 uVar4;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0xffc0;
  *(undefined2 *)(unaff_A6 + 0x1f) = 0x2a;
  candidate_1ad6e();
  candidate_9ea8();
  candidate_4520();
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  unaff_A6[0x14] = 0xe8340;
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar2;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0;
  candidate_4256();
  *unaff_A6 = 0xd2460;
  candidate_d3220();
  candidate_427a();
  sVar3 = candidate_d3436();
  iVar1 = *(int *)((short)(sVar3 << 2) + 0xe8a04);
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
