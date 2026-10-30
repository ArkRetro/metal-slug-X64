/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02be08.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2be08(void)

{
  int iVar1;
  int unaff_A6;

  *(int *)(unaff_A6 + 0x62) = *(int *)(unaff_A6 + 0x72) + *(int *)(unaff_A6 + 0x62);
  iVar1 = *(int *)(unaff_A6 + 0x76);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if (*(int *)(unaff_A6 + 0x62) <= iVar1) {
      *(int *)(unaff_A6 + 0x62) = iVar1;
      *(undefined4 *)(unaff_A6 + 0x72) = 0;
    }
  }
  else if (iVar1 <= *(int *)(unaff_A6 + 0x62)) {
    *(int *)(unaff_A6 + 0x62) = iVar1;
    *(undefined4 *)(unaff_A6 + 0x72) = 0;
  }
  return;
}
