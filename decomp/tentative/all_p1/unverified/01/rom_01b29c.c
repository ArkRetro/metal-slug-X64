/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01b29c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_1b29c(void)

{
  short in_D0w;
  ushort uVar1;
  short in_D1w;
  ushort uVar2;
  short unaff_D2w;
  short unaff_D3w;

  uVar1 = in_D0w - unaff_D2w;
  if ((short)uVar1 < 0) {
    uVar1 = -uVar1;
  }
  uVar2 = in_D1w - unaff_D3w;
  if ((short)uVar2 < 0) {
    uVar2 = -uVar2;
  }
  if ((short)uVar1 <= (short)uVar2) {
    return uVar2 + (uVar1 >> 1);
  }
  return (uVar2 >> 1) + uVar1;
}
