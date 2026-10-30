/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa956.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa956(void)

{
  char in_D0b;
  int iVar1;
  int unaff_A5;

  if ((DAT_0010f001 & 4) != 0) {
    return;
  }
  if (in_D0b == '\0') {
    iVar1 = unaff_A5 + 0x4608;
  }
  else {
    iVar1 = unaff_A5 + 0x462c;
  }
  if (*(char *)(iVar1 + 0x1f) != '\0') {
    *(char *)(iVar1 + 0x1f) = *(char *)(iVar1 + 0x1f) + -1;
    return;
  }
  return;
}
