/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05c132.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5c132(void)

{
  short sVar1;
  ushort uVar2;
  int unaff_A6;

  sVar1 = *(short *)(unaff_A6 + 0xaa);
  if (sVar1 < 0xc0) {
    uVar2 = sVar1 + 1;
  }
  else {
    uVar2 = sVar1 - 1;
  }
  *(byte *)(unaff_A6 + 0x7f) = (byte)(uVar2 >> 3) & 0x1f;
  return;
}
