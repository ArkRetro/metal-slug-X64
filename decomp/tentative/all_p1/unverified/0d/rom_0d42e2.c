/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d42e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d42e2(void)

{
  code *UNRECOVERED_JUMPTABLE;
  int unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)(unaff_A6 + 0x60) = *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + 0x60);
  uVar1 = 0;
  candidate_d5506();
  candidate_d54ee();
  candidate_d5530();
  if ((bool)uVar1) {
                    /* WARNING: Could not recover jumptable at 0x000d4302. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  candidate_44f6();
  candidate_427a();
  return;
}
