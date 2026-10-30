/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0e20.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000c0e4e: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000c0e52) */

void candidate_c0e20(void)

{
  int unaff_A5;
  bool bVar1;

  uRam0010c720 = 0;
  uRam0010c718 = 0;
  uRam0010c71a = 0;
  uRam0010c71c = 0xffff;
  uRam0010c71e = 0;
  *(undefined2 *)(unaff_A5 + 0x472c) = 0x100;
  bVar1 = false;
  func_0x000040ac(0xc0e52);
  if (bVar1) {
    candidate_c0e6e();
  }
  return;
}
