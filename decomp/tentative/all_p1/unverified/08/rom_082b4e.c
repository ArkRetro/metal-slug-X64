/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x082b4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x00082b50: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x00082b54) */
/* WARNING: Removing unreachable block (ram,0x00082b5a) */
/* WARNING: Removing unreachable block (ram,0x00082b58) */

void candidate_82b4e(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  candidate_1aeb0(0x82b54);
  if (bVar2) {
    candidate_1b08a();
    if (!bVar2) {
      sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
      if (sVar1 < 0) {
        sVar1 = -sVar1;
      }
      if (sVar1 <= *(short *)(unaff_A6 + 0x7a)) {
        return;
      }
    }
  }
  return;
}
