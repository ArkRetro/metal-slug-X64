/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b82e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b82e(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  int in_A0;
  int unaff_A6;

  uVar1 = *(ushort *)(unaff_A6 + 0x7a);
  if (uVar1 != 0) {
    uVar3 = (ushort)-((short)uVar1 < 0);
    uVar2 = (uVar3 ^ *(short *)(in_A0 + 0x40) - *(short *)(unaff_A6 + 0x40)) - uVar3;
    if (((short)uVar2 < 0) || ((ushort)((uVar3 ^ uVar1) - uVar3) < uVar2)) {
      return;
    }
  }
  return;
}
