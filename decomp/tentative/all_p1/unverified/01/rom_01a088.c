/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01a088.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1a088(void)

{
  byte *in_A0;
  byte *extraout_A0;

  while( true ) {
    if (0x11 < *in_A0) {
      __m68k_trap(0xf);
      return;
    }
    if (BYTE_ARRAY_0001a074[(short)(ushort)*in_A0] == 0) break;
    func_0x0001a372();
    in_A0 = extraout_A0;
  }
  return;
}
