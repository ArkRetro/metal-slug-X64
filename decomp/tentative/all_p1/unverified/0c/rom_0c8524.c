/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8524.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c8524(void)

{
  short unaff_D2w;
  short unaff_D3w;
  ushort unaff_D4w;
  ushort unaff_D5w;
  int in_A0;

  if (((((*(byte *)(in_A0 + 0x5f) & 0x80) == 0) && ((*(byte *)(in_A0 + 0x15) & 0x40) != 0)) &&
      ((ushort)(*(short *)(in_A0 + 0x3c) - unaff_D2w) < unaff_D4w)) &&
     ((ushort)(*(short *)(in_A0 + 0x40) - unaff_D3w) < unaff_D5w)) {
    return;
  }
  return;
}
