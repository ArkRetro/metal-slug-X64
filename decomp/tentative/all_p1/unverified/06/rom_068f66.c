/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068f66.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68f66(void)

{
  int iVar1;
  short sVar2;
  int unaff_A6;

  if ((*(char *)(unaff_A6 + 0x81) != '\0') &&
     (iVar1 = *(int *)(unaff_A6 + 0xc), *(char *)(iVar1 + 0x80) == '\0')) {
    sVar2 = *(short *)(iVar1 + 0x3c) - *(short *)(iVar1 + 0x3c);
    if (sVar2 < 0) {
      sVar2 = -sVar2;
    }
    if (0x2f < sVar2) {
      *(undefined1 *)(iVar1 + 0x80) = 1;
    }
  }
  return;
}
