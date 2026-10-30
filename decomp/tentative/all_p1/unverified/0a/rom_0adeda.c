/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0adeda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_adeda(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_1ad6e();
  *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  *unaff_A6 = 0xadefc;
  sVar1 = *(short *)(unaff_A6[3] + 0x7c);
  if (sVar1 != *(short *)(unaff_A6 + 0x1f)) {
    *(short *)(unaff_A6 + 0x1f) = sVar1;
                    /* WARNING: Could not recover jumptable at 0x000aded6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)((short)(sVar1 * 4) + 0xade66))();
    return;
  }
  return;
}
