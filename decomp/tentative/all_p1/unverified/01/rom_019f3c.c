/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019f3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_19f3c(void)

{
  byte *in_A0;
  byte *extraout_A0;
  int unaff_A5;

  do {
    if (0x11 < *in_A0) {
      __m68k_trap(0xf);
      return;
    }
                    /* WARNING: Switch is manually overridden */
    switch((ushort)*in_A0 << 2) {
    case 0:
      if (*(ushort *)(unaff_A5 + 0x4324) <= *(ushort *)(in_A0 + 2)) {
        return;
      }
      break;
    case 4:
      break;
    case 8:
      break;
    case 0xc:
      return;
    }
    candidate_1a088();
    in_A0 = extraout_A0;
  } while( true );
}
