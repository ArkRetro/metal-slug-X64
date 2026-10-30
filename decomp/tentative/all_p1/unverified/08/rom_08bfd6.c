/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08bfd6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8bfd6(void)

{
  undefined4 *puVar1;
  short sVar2;
  int unaff_A5;
  bool bVar3;

  *(int *)(unaff_A5 + 0x4590) = *(int *)(unaff_A5 + 0x4590) + 1;
  if ((*(char *)(unaff_A5 + 0x45be) != '\0') &&
     (*(short *)(unaff_A5 + 0x45b8) = *(short *)(unaff_A5 + 0x45b8) + 1,
     600 < *(ushort *)(unaff_A5 + 0x45b8))) {
    *(undefined1 *)(unaff_A5 + 0x45be) = 0;
  }
  bVar3 = false;
  if (*(char *)(unaff_A5 + 0x45bf) != '\0') {
    *(short *)(unaff_A5 + 0x45ba) = *(short *)(unaff_A5 + 0x45ba) + 1;
    bVar3 = *(ushort *)(unaff_A5 + 0x45ba) < 600;
    if (600 < *(ushort *)(unaff_A5 + 0x45ba)) {
      *(undefined1 *)(unaff_A5 + 0x45bf) = 0;
      bVar3 = false;
    }
  }
  candidate_1aeda();
  if ((bVar3) && ((*(byte *)(unaff_A5 + -0x6add) & 0x80) == 0)) {
    sVar2 = candidate_8d268();
    puVar1 = *(undefined4 **)((short)(sVar2 * 4) + 0xe541e);
    uRam0010c5ec = *puVar1;
    uRam0010c5f0 = puVar1[1];
  }
  else {
    uRam0010c5ec = 0x80008000;
    uRam0010c5f0 = 0;
  }
  bVar3 = false;
  candidate_1aeda();
  if ((bVar3) && ((*(byte *)(unaff_A5 + -0x6a2d) & 0x80) == 0)) {
    sVar2 = candidate_8d268();
    puVar1 = *(undefined4 **)((short)(sVar2 * 4) + 0xe541e);
    uRam0010c5f4 = *puVar1;
    uRam0010c5f8 = puVar1[1];
  }
  else {
    uRam0010c5f4 = 0x80008000;
    uRam0010c5f8 = 0;
  }
  return;
}
