/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066c3e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 candidate_66c3e(void)

{
  undefined4 uVar1;
  char unaff_D7b;
  byte bVar2;
  undefined1 in_CF;
  bool bVar3;

  uVar1 = candidate_66f7a();
  if (!(bool)in_CF) {
    _DAT_003c0000 = CONCAT22((short)uVar1,0xbf8);
    uVar1 = CONCAT22(0xbf8,(short)uVar1 + 0x20);
  }
  for (bVar2 = unaff_D7b - 1; bVar3 = bVar2 < 2, !bVar3; bVar2 = bVar2 - 1) {
    uVar1 = candidate_66f7a();
    if (!bVar3) {
      _DAT_003c0000 = CONCAT22((short)uVar1,0xbf9);
      uVar1 = CONCAT22(0xbf9,(short)uVar1 + 0x20);
    }
  }
  bVar3 = bVar2 == 0;
  if (bVar2 == 1) {
    uVar1 = candidate_66f7a();
    if (!bVar3) {
      _DAT_003c0000 = CONCAT22((short)uVar1,0xbfa);
      uVar1 = CONCAT22(0xbfa,(short)uVar1 + 0x20);
    }
  }
  return uVar1;
}
