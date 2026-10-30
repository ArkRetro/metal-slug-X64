/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087aa0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87aa0(void)

{
  ushort uVar1;
  int unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  if ((*(byte *)(unaff_A6 + 0x8b) & 0x10) == 0) {
    candidate_87954();
    if ((bool)in_CF) {
      return;
    }
  }
  else if ((((*(byte *)(unaff_A6 + 0x8b) & 0x20) == 0) &&
           (uVar1 = *(ushort *)(*(int *)(unaff_A6 + 0x6e) + 0x40),
           bVar2 = uVar1 < *(ushort *)(unaff_A6 + 0x40),
           (short)*(ushort *)(unaff_A6 + 0x40) < (short)uVar1)) && (candidate_1af34(), !bVar2)) {
    return;
  }
  return;
}
