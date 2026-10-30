/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01495c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_1495c(void)

{
  int iVar1;
  int extraout_D1;
  int iVar2;
  uint unaff_D6;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;

  candidate_128da();
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  iVar1 = *(int *)(unaff_A6 + 0x2c);
  if (*(int *)(unaff_A6 + 0x38) != 0) {
    iVar1 = *(int *)(*(int *)(unaff_A6 + 0x38) + 0x18) + iVar1;
  }
  candidate_13968();
  if ((unaff_D6 & 1) != 0) {
    iVar2 = extraout_D1;
    if (extraout_A0 != 0) {
      iVar2 = *(int *)(extraout_A0 + 0x18) + extraout_D1;
    }
    return CONCAT22((short)((uint)iVar1 >> 0x10),(short)((uint)(iVar2 - iVar1) >> 0x10));
  }
  return iVar1;
}
