/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05777e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_5777e(void)

{
  uint uVar1;
  uint in_D0;
  ushort uVar3;
  uint uVar2;
  ushort uVar4;

  uVar3 = 0;
  uVar2 = in_D0 & 0xffff;
  if (uVar2 != 0) {
    uVar1 = uVar2 / 10;
    uVar3 = (ushort)(uVar2 % 10);
    uVar2 = uVar1;
    if (uVar1 != 0) {
      uVar2 = uVar1 / 10;
      uVar3 = (short)(uVar1 % 10) << 4 | uVar3;
      if (uVar2 != 0) {
        uVar1 = uVar2 % 10;
        uVar4 = (short)((uVar2 / 10) % 10) << 0xc;
        uVar2 = CONCAT22((short)((uVar2 / 10) / 10),uVar4);
        uVar3 = uVar4 | (short)uVar1 << 8 | uVar3;
      }
    }
  }
  return CONCAT22((short)(uVar2 >> 0x10),uVar3);
}
