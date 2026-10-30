/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ae00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_1ae00(void)

{
  undefined2 in_D0w;
  uint in_A0;
  int extraout_A0;
  ushort *extraout_A0_00;
  bool bVar1;

  bVar1 = in_A0 < 0x1014c4;
  if (((in_A0 == 0x1014c4) || (bVar1 = in_A0 < 0x101574, in_A0 == 0x101574)) &&
     (in_D0w = func_0x000040ac(), bVar1)) {
    in_D0w = *(undefined2 *)(extraout_A0 + 0x70);
    candidate_ca2ce();
    if (((*(byte *)(extraout_A0 + 0x15) & 0x40) != 0) && ((*extraout_A0_00 & 2) != 0)) {
      return in_D0w;
    }
  }
  return in_D0w;
}
