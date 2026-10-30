/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0decf6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_decf6(void)

{
  ushort in_D0w;
  ushort *puVar1;

  if ((short)in_D0w < 0) {
    in_D0w = -in_D0w;
  }
  for (puVar1 = (ushort *)0xded12; *puVar1 < in_D0w; puVar1 = puVar1 + 2) {
  }
  return puVar1[1];
}
