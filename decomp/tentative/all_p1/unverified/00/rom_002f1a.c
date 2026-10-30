/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002f1a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_2f1a(void)

{
  ushort in_D0w;
  ushort in_D1w;

  if ((short)in_D0w < 0) {
    in_D0w = -in_D0w;
  }
  if ((short)in_D1w < 0) {
    in_D1w = -in_D1w;
  }
  if (in_D0w < in_D1w) {
    return in_D1w + (in_D0w >> 1);
  }
  return (in_D1w >> 1) + in_D0w;
}
