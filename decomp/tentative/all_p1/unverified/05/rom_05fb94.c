/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05fb94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5fb94(void)

{
  ushort uVar1;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x3c) <
      (short)(*(short *)(unaff_A6 + 0x76) + *(short *)(unaff_A6 + 0x7a))) {
    uVar1 = candidate_260c8();
    *(uint *)(unaff_A6 + 0x62) = (uint)uVar1 << 8;
  }
  else {
    uVar1 = candidate_260c8();
    *(uint *)(unaff_A6 + 0x62) = (uint)uVar1 * -0x100;
  }
  candidate_5fad8();
  candidate_4256();
  return;
}
