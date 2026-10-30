/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5570.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5570(void)

{
  byte *pbVar1;
  int unaff_A6;

  if (-1 < (char)*(byte *)(unaff_A6 + 0x8d)) {
    pbVar1 = (byte *)(*(int *)(unaff_A6 + 0xc) + 0x8b);
    *pbVar1 = *pbVar1 & ~('\x01' << (*(byte *)(unaff_A6 + 0x8d) & 7));
    *(undefined1 *)(unaff_A6 + 0x8d) = 0xff;
  }
  return;
}
