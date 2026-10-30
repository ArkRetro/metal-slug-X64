/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04465e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_19dca(void)

{
  byte bVar1;
  uint uVar2;
  undefined4 in_D0;
  int unaff_A5;

  bVar1 = (char)in_D0 - 3;
  if (8 < bVar1) {
    return 0;
  }
  uVar2 = CONCAT22((short)((uint)in_D0 >> 0x10),(ushort)bVar1) & 0xffff000f;
  return CONCAT31((int3)(uVar2 >> 8),*(undefined1 *)(unaff_A5 + 0x4326 + (int)(short)uVar2));
}
