/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b2144.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000b2180: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000b2186) */

void candidate_b2144(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  byte bVar4;
  int unaff_A5;
  undefined4 *unaff_A6;

  if (0x13f < *(ushort *)(candidate_698c + unaff_A5 + 2)) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  uVar3 = *(short *)((int)unaff_A6 + 0x72) + 0x400;
  if ((short)uVar3 < 0) {
    uVar3 = *(short *)((int)unaff_A6 + 0x72) + 0x800;
  }
  *(ushort *)((int)unaff_A6 + 0x72) = uVar3;
  if ((short)uVar3 < 0) {
    uVar3 = ~uVar3;
  }
  bVar4 = (byte)(((uint)uVar3 << 6) >> 0x10) & 0x1f;
  if (sRam00106a82 == 0) {
    return;
  }
  if (sRam00106a86 == 0xad1) {
    if ((bVar4 != bRam00106a88) && (bRam00106a88 = bVar4, sRam00106a84 != 0xad1)) {
      bRam00106a89 = bRam00106a89 | 0x80;
    }
  }
  else {
    sRam00106a86 = 0xad1;
    bRam00106a89 = bRam00106a89 | 0x80;
    bRam00106a88 = bVar4;
  }
  return;
}
