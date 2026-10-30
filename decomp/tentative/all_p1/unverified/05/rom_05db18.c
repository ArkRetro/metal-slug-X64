/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05db18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5db18(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *unaff_A6 = 0x5db42;
  candidate_406b6();
                    /* WARNING: Could not recover jumptable at 0x0005db4a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*unaff_A6)();
  return;
}
