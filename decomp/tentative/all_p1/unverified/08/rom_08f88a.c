/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f88a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f88a(void)

{
  byte bVar1;
  int iVar2;
  int unaff_A6;

  iVar2 = 0x8fef0;
  if ((*(byte *)(unaff_A6 + 0x93) & 2) != 0) {
    iVar2 = 0x8ff14;
  }
  if ((*(byte *)(unaff_A6 + 0x93) & 0x20) != 0) {
    iVar2 = 0x8ff38;
  }
  bVar1 = *(byte *)(*(int *)(unaff_A6 + 0xc) + 0x8d);
  if (bVar1 == *(byte *)(unaff_A6 + 0x8d)) {
    *(byte *)(unaff_A6 + 0x8d) = bVar1;
  }
  else {
    *(byte *)(unaff_A6 + 0x8d) = bVar1;
    if (*(int *)(iVar2 + (short)((bVar1 & 0xf) << 2)) != 0) {
      return;
    }
  }
  return;
}
