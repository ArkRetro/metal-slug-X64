/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08cbca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8cbca(void)

{
  short sVar1;
  short sVar2;
  int unaff_A6;

  if (*(char *)(*(int *)(unaff_A6 + 0x82) + 1) != '\0') {
    sVar1 = 7;
    sVar2 = 0;
    do {
      if (*(int *)(sVar2 + 0x10c5c2) == 0) {
        *(int *)(sVar2 + 0x10c5c2) = unaff_A6;
        return;
      }
      sVar2 = sVar2 + 4;
      sVar1 = sVar1 + -1;
    } while (sVar1 != -1);
  }
  return;
}
