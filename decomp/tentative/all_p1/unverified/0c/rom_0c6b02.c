/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c6b02.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_c6b02(void)

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
  if (iVar1 < 0) {
    if (cVar2 != '\0') {
      *(undefined4 *)(unaff_A6 + 0x62) = 0;
    }
  }
  else if (cVar2 == '\0') {
    *(undefined4 *)(unaff_A6 + 0x62) = 0;
  }
  return iVar1;
}
