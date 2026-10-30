/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f3e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f3e2(void)

{
  int iVar1;
  byte bVar2;
  short sVar3;
  int unaff_A6;

  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined2 *)(unaff_A6 + 0x1e) = *(undefined2 *)(iVar1 + 0x1e);
  sVar3 = (short)*(char *)(iVar1 + 0x8b);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar3 = -sVar3;
  }
  *(short *)(unaff_A6 + 0x28) = sVar3;
  *(short *)(unaff_A6 + 0x2c) = (short)*(char *)(iVar1 + 0x8c);
  bVar2 = *(byte *)(unaff_A6 + 0x8a) & 0xf;
  if ((bVar2 == 0) || (7 < bVar2)) {
    *(short *)(unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x1e) + -1;
  }
  candidate_44f6();
  return;
}
