/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cb878.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cb878(void)

{
  undefined1 uVar1;
  short *psVar2;
  int iVar3;
  int unaff_A6;

  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x6e) != 0) {
    *(short *)(unaff_A6 + 0x6e) = *(short *)(unaff_A6 + 0x6e) + -1;
  }
  candidate_44f6();
  psVar2 = (short *)0xe81aa;
  iVar3 = 0xe81e2;
  uVar1 = *(undefined1 *)(unaff_A6 + 0x16);
  while (*psVar2 != -1) {
    *(char *)(unaff_A6 + 0x16) = (char)*psVar2;
    psVar2 = psVar2 + 3;
    iVar3 = iVar3 + 2;
    candidate_454c(psVar2,iVar3);
  }
  *(undefined1 *)(unaff_A6 + 0x16) = uVar1;
  return;
}
