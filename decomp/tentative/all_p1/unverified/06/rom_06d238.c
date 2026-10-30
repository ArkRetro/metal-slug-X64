/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d238.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d238(void)

{
  ushort uVar1;
  short extraout_D1w;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x80) & 2) == 0) {
    candidate_29f2();
  }
  uVar1 = candidate_29c6();
  *(int *)(unaff_A6 + 0x72) = (int)(short)(extraout_D1w + (uVar1 & 0x3f)) << 8;
  return;
}
