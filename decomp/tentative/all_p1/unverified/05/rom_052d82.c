/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x052d82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_52d82(void)

{
  int iVar1;
  ushort uVar2;
  undefined1 uVar3;
  short sVar4;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x82) != '\0') {
    *(char *)(unaff_A6 + 0x82) = *(char *)(unaff_A6 + 0x82) + -1;
  }
  iVar1 = *(int *)(unaff_A6 + 0x6e);
  if ((-1 < iVar1) && (*(char *)(unaff_A6 + 0x89) != '\0')) {
    uVar2 = *(short *)(unaff_A6 + 0x40) - *(short *)(iVar1 + 0x40);
    if ((*(short *)(unaff_A6 + 0x40) != *(short *)(iVar1 + 0x40)) && (-1 < (short)uVar2)) {
      sVar4 = *(short *)(unaff_A6 + 0x3c) - *(short *)(iVar1 + 0x3c);
      if (sVar4 < 0) {
        sVar4 = -sVar4;
      }
      if ((uVar2 <= (ushort)(sVar4 * 2)) && (-1 < *(int *)(unaff_A6 + 0x6e))) {
        candidate_29c6();
        uVar3 = candidate_2eb4();
        *(undefined1 *)(unaff_A6 + 0x88) = uVar3;
        return;
      }
    }
  }
  return;
}
