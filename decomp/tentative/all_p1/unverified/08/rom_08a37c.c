/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a37c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a37c(void)

{
  int iVar1;
  int *unaff_A6;

  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    return;
  }
  if (*(char *)(unaff_A6 + 0x22) == '\0') {
    candidate_8a3d4();
  }
  else {
    candidate_8a3f6();
  }
  if (*(char *)((int)unaff_A6 + 0x89) == '\0') {
    return;
  }
  iVar1 = *unaff_A6;
  candidate_8a5dc();
  if (iVar1 != *unaff_A6) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 3;
  }
  return;
}
