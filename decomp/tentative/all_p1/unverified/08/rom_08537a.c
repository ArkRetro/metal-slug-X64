/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08537a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_8537a(void)

{
  int iVar1;
  byte bVar2;
  int unaff_A6;

  bVar2 = *(byte *)(unaff_A6 + 0x8f);
  if (-1 < (char)bVar2) {
    iVar1 = *(int *)(unaff_A6 + 0xc);
    bVar2 = *(byte *)(unaff_A6 + 0x8f);
    *(byte *)(iVar1 + 0x8c) = *(byte *)(iVar1 + 0x8c) & ~('\x01' << (bVar2 & 7));
    if (((*(byte *)(unaff_A6 + 0x5f) & 0x80) != 0) &&
       (*(byte *)(iVar1 + 0x8e) = *(byte *)(iVar1 + 0x8e) & ~('\x01' << (bVar2 & 7)),
       *(char *)(iVar1 + 0x8e) == '\0')) {
      bVar2 = candidate_854f8();
    }
    *(undefined1 *)(unaff_A6 + 0x8f) = 0xff;
  }
  return bVar2;
}
