/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x052cda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_52cda(void)

{
  byte bVar1;
  undefined1 uVar2;
  int unaff_A6;
  ushort uVar3;

  uVar3 = *(short *)(unaff_A6 + 0x3c) - 0x20;
  if (uVar3 < 0x100) {
    uVar2 = (undefined1)(uVar3 >> 8);
    uVar3 = CONCAT11(uVar2,*(char *)(unaff_A6 + 0x8d));
    if ((*(char *)(unaff_A6 + 0x8d) == '\0') &&
       (bVar1 = *(byte *)(unaff_A6 + 0x8b) & *(byte *)(unaff_A6 + 0x89),
       uVar3 = CONCAT11(uVar2,bVar1), bVar1 != 0)) {
      return uVar3;
    }
  }
  return uVar3;
}
