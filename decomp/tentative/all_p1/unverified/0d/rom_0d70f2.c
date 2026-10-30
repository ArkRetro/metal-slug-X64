/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d70f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d70f2(void)

{
  int iVar1;
  char cVar2;
  char cVar3;
  int unaff_A6;

  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(short *)(unaff_A6 + 0x1e) = *(short *)(iVar1 + 0x1e) + *(short *)(unaff_A6 + 0x80);
  cVar2 = *(char *)(iVar1 + 0x7b);
  cVar3 = *(char *)(iVar1 + 0x7c);
  *(char *)(unaff_A6 + 0x7b) = cVar2;
  *(char *)(unaff_A6 + 0x7c) = cVar3;
  *(short *)(unaff_A6 + 0x28) = (short)cVar2;
  *(short *)(unaff_A6 + 0x2c) = (short)cVar3;
  return;
}
