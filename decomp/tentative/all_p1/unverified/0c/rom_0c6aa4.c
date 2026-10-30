/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6aa4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c6aa4(void)

{
  int iVar1;
  char cVar2;
  int unaff_A6;

  cVar2 = '\0';
  if (*(int *)(unaff_A6 + 0x78) < 0) {
    cVar2 = -1;
  }
  *(int *)(unaff_A6 + 0x62) = *(int *)(unaff_A6 + 0x78) + *(int *)(unaff_A6 + 0x62);
  iVar1 = *(int *)(unaff_A6 + 0x62);
  if (cVar2 != '\0') {
    iVar1 = -iVar1;
  }
  if ((*(int *)(unaff_A6 + 0x80) < iVar1) &&
     (*(undefined4 *)(unaff_A6 + 0x62) = *(undefined4 *)(unaff_A6 + 0x80), cVar2 != '\0')) {
    *(int *)(unaff_A6 + 0x62) = -*(int *)(unaff_A6 + 0x62);
  }
  iVar1 = *(int *)(unaff_A6 + 0x62);
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  *(int *)(unaff_A6 + 0x84) = iVar1 + *(int *)(unaff_A6 + 0x84);
  return;
}
