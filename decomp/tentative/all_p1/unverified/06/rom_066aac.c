/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066aac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_66aac(void)

{
  undefined2 in_D0w;
  undefined2 uVar1;
  int unaff_A3;

  candidate_66a94();
  if (*(code **)(unaff_A3 + 4) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00066abe. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(unaff_A3 + 4))();
    return uVar1;
  }
  return in_D0w;
}
