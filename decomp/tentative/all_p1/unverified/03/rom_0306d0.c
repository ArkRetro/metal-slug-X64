/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0306d0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_306d0(void)

{
  undefined4 extraout_A0;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x84) < *(short *)(unaff_A6 + 0x82)) {
    candidate_30152();
    candidate_29f2(extraout_A0);
    candidate_3010a();
  }
  candidate_304d6();
  candidate_30548();
  *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) + -1;
  if (*(short *)(unaff_A6 + 0x80) == 0) {
    _DAT_0010c5fc = _DAT_0010c5fc + -1;
  }
  else {
    _DAT_0010c600 = _DAT_0010c600 + -1;
  }
  candidate_30300();
  return;
}
