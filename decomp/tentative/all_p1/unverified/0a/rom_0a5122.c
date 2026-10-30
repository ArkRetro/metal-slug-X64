/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5122.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5122(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((*(ushort *)(unaff_A5 + -0x7ff2) & 3) == 0) {
    if (*(char *)((int)unaff_A6 + 0x8a) == *(char *)(unaff_A6 + 0x22)) {
      *(byte *)((int)unaff_A6 + 0x89) = *(byte *)(unaff_A6 + 0x22) >> 1;
                    /* WARNING: Could not recover jumptable at 0x000a5164. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)((int)unaff_A6 + 0x7e))();
      return;
    }
    *(byte *)(unaff_A6 + 0x22) =
         (-((char)(*(char *)((int)unaff_A6 + 0x8a) - *(char *)(unaff_A6 + 0x22)) < '\0') & 0xfeU) +
         1 + *(char *)(unaff_A6 + 0x22);
  }
  candidate_1abc8();
  candidate_427a();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
    *unaff_A6 = FUN_000a52d6;
  }
  return;
}
