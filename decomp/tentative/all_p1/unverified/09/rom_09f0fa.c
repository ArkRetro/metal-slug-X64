/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09f0fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f0fa(void)

{
  ushort uVar1;
  int unaff_A6;
  bool bVar2;

  uVar1 = *(ushort *)(unaff_A6 + 0x62);
  if ((uVar1 == 0) && (uVar1 = 2, (*(byte *)(unaff_A6 + 0x15) & 1) == 0)) {
    uVar1 = 0xfffe;
  }
  bVar2 = CARRY2(uVar1,uVar1);
  if ((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0) {
    candidate_14850();
  }
  else {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xdf;
    candidate_14850();
    if (bVar2) {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x20;
    }
    else {
      *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) | 0x20;
    }
  }
  return;
}
