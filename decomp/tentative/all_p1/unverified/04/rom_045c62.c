/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x045c62.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_45c62(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  candidate_446f0();
  candidate_1ac38();
  candidate_427a();
  if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
    candidate_120f8();
    candidate_4256();
    *unaff_A6 = candidate_45688;
  }
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *(char *)(unaff_A6 + 0x23) = (char)*psVar1;
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
