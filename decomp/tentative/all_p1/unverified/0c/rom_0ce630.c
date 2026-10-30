/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ce630.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ce630(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A6;

  uVar1 = candidate_29c6();
  sVar2 = (uVar1 & 7) << 2;
  *(undefined2 *)(unaff_A6 + 0x9a) = *(undefined2 *)(sVar2 + 0xceb8c);
  *(undefined2 *)(unaff_A6 + 0x92) = *(undefined2 *)(sVar2 + 0xceb8e);
  *(short *)(unaff_A6 + 0x92) = *(short *)(unaff_A6 + 0x92) + 0x2c;
  return;
}
