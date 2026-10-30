/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a57ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_a57ba(void)

{
  byte bVar1;
  char in_D0b;
  short sVar2;
  ushort in_D1w;
  int unaff_A6;

  bVar1 = in_D0b + *(char *)(unaff_A6 + 0xa5);
  *(byte *)(unaff_A6 + 0xa5) = bVar1;
  sVar2 = *(short *)((short)((ushort)bVar1 * 2) + 0x29fc);
  if (in_D1w != 0) {
    sVar2 = sVar2 << (in_D1w & 0x3f);
  }
  return sVar2;
}
