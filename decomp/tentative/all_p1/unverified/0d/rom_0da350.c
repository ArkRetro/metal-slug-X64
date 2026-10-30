/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0da350.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_da350(void)

{
  ushort uVar1;
  undefined4 uVar2;

  uVar2 = candidate_da2a2();
  uVar1 = (short)((short)uVar2 + 8) >> 3;
  if ((uVar1 & 0x10) == 0) {
    uVar1 = -uVar1;
  }
  return CONCAT22((short)((uint)uVar2 >> 0x10),uVar1) & 0xffff001f;
}
