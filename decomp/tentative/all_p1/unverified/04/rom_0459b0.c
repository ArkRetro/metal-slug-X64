/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0459b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_459b0(void)

{
  short *psVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  if ((bool)in_CF) {
    candidate_12192();
    *(undefined1 *)(*(int *)((int)unaff_A6 + 0x72) + 0x6e) = 4;
    *unaff_A6 = candidate_459de;
  }
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *(char *)(unaff_A6 + 0x23) = (char)*psVar1;
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
