/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cca24.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cca24(void)

{
  int iVar1;
  undefined2 uVar2;
  short sVar3;
  int *unaff_A6;
  bool bVar4;

  *(undefined2 *)((int)unaff_A6 + 0x9a) = 0xa0;
  *(undefined2 *)((int)unaff_A6 + 0x92) = 400;
  *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) + 0x2c;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  *(undefined2 *)(unaff_A6 + 0x2a) = 0x200;
  *(undefined1 *)(unaff_A6 + 0x20) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 600;
  candidate_4256();
  *unaff_A6 = 0xcca7a;
  bVar4 = false;
  candidate_ce448();
  if (bVar4) {
    *(undefined1 *)(unaff_A6 + 0x20) = 4;
  }
  candidate_ce420();
  sVar3 = candidate_ce58e();
  iVar1 = *(int *)((short)(sVar3 << 2) + 0xceaa0);
  if (iVar1 != 0) {
    *unaff_A6 = iVar1;
  }
  candidate_ce5da();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(undefined1 *)((int)unaff_A6 + 0x73) = 2;
  }
  return;
}
