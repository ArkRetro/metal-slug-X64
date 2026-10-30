/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ab9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_9ab9a(void)

{
  int iVar1;
  uint uVar2;
  int extraout_A0;
  int extraout_A0_00;
  undefined1 in_CF;
  bool bVar3;
  int iStack_4;

  uVar2 = candidate_18f16();
  if (!(bool)in_CF) {
    iStack_4 = -1;
    bVar3 = false;
    candidate_1aeb0();
    if ((bVar3) && (candidate_9ab7e(), bVar3)) {
      iStack_4 = extraout_A0;
    }
    bVar3 = false;
    candidate_1aeb0();
    iVar1 = iStack_4;
    if ((((bVar3) && (candidate_9ab7e(), bVar3)) && (iVar1 = extraout_A0_00, -1 < iStack_4)) &&
       (iVar1 = extraout_A0_00, *(ushort *)(extraout_A0_00 + 0x3c) < *(ushort *)(iStack_4 + 0x3c)))
    {
      iVar1 = iStack_4;
    }
    iStack_4 = iVar1;
    uVar2 = 0;
    if (-1 < iStack_4) {
      return (uint)(iStack_4 != 0x1014c4);
    }
  }
  return uVar2;
}
