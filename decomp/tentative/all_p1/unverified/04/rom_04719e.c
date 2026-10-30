/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04719e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4719e(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  if (*(char *)((int)unaff_A6 + 0x8e) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x471d0;
  candidate_1436a();
  candidate_427a();
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
