/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aeb40.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aeb40(void)

{
  short sVar1;
  ushort in_D0w;
  short unaff_D2w;
  short unaff_D3w;
  int unaff_A5;
  int unaff_A6;

  if (*(ushort *)(unaff_A6 + 0x92) < in_D0w) {
    *(ushort *)(unaff_A6 + 0x92) = in_D0w;
    sVar1 = *(short *)(unaff_A5 + 0x6980);
    *(short *)(unaff_A6 + 0x8e) = unaff_D2w - *(short *)(unaff_A5 + 0x697c);
    *(short *)(unaff_A6 + 0x90) = unaff_D3w - sVar1;
  }
  return;
}
