/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca21e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000ca270: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000ca274) */
/* WARNING: Removing unreachable block (ram,0x000ca2a2) */
/* WARNING: Removing unreachable block (ram,0x000ca2ba) */
/* WARNING: Removing unreachable block (ram,0x000ca2be) */
/* WARNING: Removing unreachable block (ram,0x000ca284) */
/* WARNING: Removing unreachable block (ram,0x000ca294) */
/* WARNING: Removing unreachable block (ram,0x000ca29e) */
/* WARNING: Removing unreachable block (ram,0x000ca2c2) */

void candidate_ca21e(void)

{
  code *extraout_A1;
  int unaff_A5;
  bool bVar1;

  bVar1 = false;
  if (*(int *)(unaff_A5 + 0x69f8) == 0) {
    func_0x000ca1b8();
  }
  else {
    func_0x000040ac();
    if (bVar1) {
      (*extraout_A1)();
      candidate_ca202();
    }
  }
  *(undefined2 *)(unaff_A5 + 0x69ee) = 0;
  *(undefined2 *)(unaff_A5 + 0x69ec) = 0;
  return;
}
