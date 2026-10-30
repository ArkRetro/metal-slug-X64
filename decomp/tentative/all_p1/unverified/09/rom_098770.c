/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x098770.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_98770(void)

{
  int unaff_A6;

  *(int *)(unaff_A6 + 0x30) =
       (*(short *)(unaff_A6 + 0x72) * 0x100 - *(int *)(unaff_A6 + 0x30) >> 4) +
       *(int *)(unaff_A6 + 0x30);
  *(int *)(unaff_A6 + 0x34) =
       (*(short *)(unaff_A6 + 0x74) * 0x100 - *(int *)(unaff_A6 + 0x34) >> 4) +
       *(int *)(unaff_A6 + 0x34);
  candidate_1ac38();
  return;
}
