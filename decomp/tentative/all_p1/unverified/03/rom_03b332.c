/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03b332.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_3b332(void)

{
  short in_D0w;
  short in_D1w;
  uint unaff_D2;
  uint uVar1;
  ushort unaff_D3w;
  int iVar2;
  short sVar3;

  uVar1 = unaff_D2;
  if ((int)unaff_D2 < 0) {
    uVar1 = -unaff_D2;
  }
  if ((short)unaff_D3w < 2) {
    unaff_D3w = 2;
  }
  sVar3 = (short)(0x10000 / unaff_D3w);
  iVar2 = (int)((uint)unaff_D3w * (uVar1 >> 0x10) * 0x10000 + (uint)unaff_D3w * (uVar1 & 0xffff)) >>
          1;
  if ((int)unaff_D2 < 0) {
    iVar2 = -iVar2;
  }
  return CONCAT44((int)in_D0w * (int)sVar3,(int)in_D1w * (int)sVar3 - iVar2);
}
