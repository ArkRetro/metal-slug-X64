/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046fae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_46fae(void)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  int unaff_A6;
  bool bVar4;

  bVar4 = *(short *)(unaff_A6 + 0x40) != 0;
  sVar1 = candidate_1495c();
  if (!bVar4) {
    return 0;
  }
  uVar3 = *(ushort *)(unaff_A6 + 0x8a);
  uVar2 = 0;
  while( true ) {
    if ((ushort)-sVar1 <= uVar3) {
      return uVar2;
    }
    uVar2 = uVar2 + 1;
    if (2 < uVar2) break;
    uVar3 = *(short *)(unaff_A6 + 0x8a) + uVar3;
  }
  return 3;
}
