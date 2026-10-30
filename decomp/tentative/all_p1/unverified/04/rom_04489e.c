/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04489e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4489e(void)

{
  byte bVar1;
  ushort in_D0w;
  int unaff_A6;

  bVar1 = *(byte *)(unaff_A6 + 0x84) & 0x7f;
  if (bVar1 == *(byte *)(*(int *)(unaff_A6 + 0x72) + 0x6e +
                        (int)(short)(ushort)*(byte *)(unaff_A6 + 0x83))) {
    if ((short)in_D0w < 0) {
      in_D0w = (ushort)bVar1;
    }
    *(byte *)(*(int *)(unaff_A6 + 0x72) + 0x6e + (int)(short)(ushort)*(byte *)(unaff_A6 + 0x83)) =
         (byte)in_D0w | 0x80;
    *(byte *)(unaff_A6 + 0x84) = bVar1 | 0x80;
  }
  return;
}
