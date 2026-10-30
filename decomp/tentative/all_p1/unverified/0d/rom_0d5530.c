/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d5530.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_d5530(void)

{
  byte bVar1;
  undefined4 in_D0;
  uint uVar2;
  int in_A0;
  int unaff_A6;

  bVar1 = *(byte *)(*(int *)(unaff_A6 + 0xc) + (int)*(short *)(unaff_A6 + 0x8e));
  *(undefined1 *)(*(int *)(unaff_A6 + 0xc) + (int)*(short *)(unaff_A6 + 0x8e)) = 0;
  uVar2 = CONCAT22((short)((uint)in_D0 >> 0x10),(ushort)bVar1) & 0xffff000f;
  if ((bVar1 & 0xf) != 0) {
    uVar2 = *(uint *)(in_A0 + (short)((short)uVar2 << 2));
    if (uVar2 != 0) {
      return uVar2;
    }
  }
  return uVar2;
}
