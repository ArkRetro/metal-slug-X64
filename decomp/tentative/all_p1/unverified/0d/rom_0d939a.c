/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d939a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_d939a(void)

{
  short sVar1;
  short sVar2;
  ushort in_D0w;
  ushort uVar3;
  int unaff_A6;

  sVar2 = (in_D0w & 0xff) * 2;
  sVar1 = *(short *)(sVar2 + 0x2a7c);
  uVar3 = *(ushort *)(unaff_A6 + 0xa8) & 0x7fff;
  if (uVar3 < 0x101) {
    uVar3 = 0x100;
  }
  *(int *)(unaff_A6 + 0x34) =
       (((int)*(short *)(sVar2 + 0x29fc) * (int)(short)uVar3 >> 8) << 0x10) >> 8;
  *(int *)(unaff_A6 + 0x30) = (((int)sVar1 * (int)(short)uVar3 >> 8) << 0x10) >> 8;
  return uVar3;
}
