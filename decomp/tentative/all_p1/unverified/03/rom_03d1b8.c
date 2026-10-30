/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d1b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3d1b8(void)

{
  byte bVar1;
  ushort unaff_D3w;
  byte *in_A0;
  byte *pbVar2;
  byte *pbVar3;

  while( true ) {
    while( true ) {
      while( true ) {
        while( true ) {
          if (*in_A0 != 0x5c) {
            return;
          }
          pbVar2 = in_A0 + 2;
          bVar1 = in_A0[1];
          if (bVar1 != 0x6e) break;
          unaff_D3w = unaff_D3w + 1;
          in_A0 = pbVar2;
          if (0x74ff < unaff_D3w) {
            __m68k_trap(0xf);
          }
        }
        if (bVar1 != 99) break;
        in_A0 = in_A0 + 3;
      }
      if (bVar1 != 0x70) break;
      in_A0 = in_A0 + 3;
    }
    if (bVar1 != 0x6c) break;
    pbVar3 = in_A0 + 3;
    in_A0 = in_A0 + 4;
    unaff_D3w = CONCAT11((char)((ushort)((ushort)*pbVar2 << 5) >> 8),
                         *pbVar3 + (char)((ushort)*pbVar2 << 5)) + 0x7000;
    if (0x74ff < unaff_D3w) {
      __m68k_trap(0xf);
    }
  }
  return;
}
