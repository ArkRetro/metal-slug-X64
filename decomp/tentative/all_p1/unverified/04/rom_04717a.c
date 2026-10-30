/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04717a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4717a(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  candidate_446f0();
  if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
    *unaff_A6 = candidate_4719e;
  }
  func_0x000143e4();
  candidate_427a();
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
