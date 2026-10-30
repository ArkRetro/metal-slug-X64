/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085026.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_85026(void)

{
  ushort uVar1;
  ushort in_D0w;
  ushort uVar2;
  ushort in_D1w;
  ushort unaff_D2w;
  ushort *puVar3;
  ushort *puVar4;

  if (in_D0w == 0) {
    in_D0w = 1;
  }
  else if ((short)in_D0w < 0) {
    in_D0w = -in_D0w;
  }
  if ((short)in_D1w < 0) {
    in_D1w = -in_D1w;
  }
  if ((short)in_D0w <= (short)in_D1w) {
    return in_D0w;
  }
  puVar3 = (ushort *)0x85070;
  do {
    puVar4 = puVar3 + 1;
    uVar1 = *puVar3;
    if (uVar1 == 0) {
      uVar2 = 0x4cc2;
      break;
    }
    puVar3 = puVar3 + 2;
    uVar2 = *puVar4;
  } while ((ushort)(((uint)in_D1w << 0x10) / (uint)in_D0w) < uVar1);
  return unaff_D2w + (short)((uint)uVar2 * (uint)unaff_D2w >> 0x10);
}
