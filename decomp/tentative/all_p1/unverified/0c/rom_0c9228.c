/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c9228.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_c9228(void)

{
  byte bVar1;
  byte *pbVar2;
  int unaff_A5;
  int unaff_A6;

  pbVar2 = (byte *)((int)*(short *)(unaff_A6 + 0x6e) + unaff_A5 + 0x69da);
  bVar1 = *pbVar2;
  *pbVar2 = *pbVar2 + 1;
  if ((bVar1 & 0x3f) == 0) {
    return 1;
  }
  if ((bVar1 & 0x3f) == 0x30) {
    return 0xffffffff;
  }
  return 0;
}
