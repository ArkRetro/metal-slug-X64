/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x084fd6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_84fd6(void)

{
  uint in_D0;
  ushort in_D1w;

  if (in_D1w == 0) {
    return in_D0;
  }
  return CONCAT22((short)((in_D0 >> 0x10) / (uint)in_D1w),
                  (short)(CONCAT22((short)((in_D0 >> 0x10) % (uint)in_D1w),(short)in_D0) /
                         (uint)in_D1w));
}
