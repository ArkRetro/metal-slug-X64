/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c804a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c804a(void)

{
  int iVar1;
  int unaff_A5;

  iVar1 = (*(int *)(unaff_A5 + 0x69c8) >> 4) - *(int *)(unaff_A5 + 0x69c8);
  if (iVar1 < 0x80001) {
    if (iVar1 < -0x80000) {
      iVar1 = -0x80000;
    }
  }
  else {
    iVar1 = 0x80000;
  }
  *(int *)(unaff_A5 + 0x69c8) = iVar1;
  *(uint *)(unaff_A5 + 0x69cc) = *(int *)(unaff_A5 + 0x69c8) + 0x8000U & 0xffff0000;
  return;
}
