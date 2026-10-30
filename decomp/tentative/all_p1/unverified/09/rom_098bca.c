/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x098bca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_98bca(void)

{
  int unaff_A6;
  undefined8 uVar1;

  candidate_2eb4();
  uVar1 = candidate_2e7c();
  *(int *)(unaff_A6 + 0x30) =
       ((int)((ulonglong)uVar1 >> 0x20) - *(int *)(unaff_A6 + 0x30) >> 4) +
       *(int *)(unaff_A6 + 0x30);
  *(int *)(unaff_A6 + 0x34) =
       ((int)uVar1 - *(int *)(unaff_A6 + 0x34) >> 4) + *(int *)(unaff_A6 + 0x34);
  candidate_1ac38();
  return;
}
