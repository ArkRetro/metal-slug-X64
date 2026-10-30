/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09cd36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9cd36(void)

{
  ushort uVar2;
  int iVar1;
  int unaff_A6;

  uVar2 = candidate_260c8();
  iVar1 = (int)(short)uVar2;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    iVar1 = -iVar1;
  }
  *(int *)(unaff_A6 + 0x62) = iVar1 << 8;
  uVar2 = (uVar2 & 0x700) >> 1;
  if (uVar2 == 0) {
    uVar2 = 0x80;
  }
  *(ushort *)(unaff_A6 + 0x26) = uVar2;
  return;
}
