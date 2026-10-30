/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03663e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3663e(void)

{
  byte bVar1;
  short sVar2;
  ushort uVar3;
  int unaff_A6;

  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  if (-1 < *(char *)(unaff_A6 + 0x9b)) {
    sVar2 = candidate_29f2();
    uVar3 = sVar2 - 3;
    bVar1 = (char)uVar3 + *(char *)(unaff_A6 + 0x9b);
    if ((char)bVar1 < '\0') {
      uVar3 = 0;
    }
    if (0x10 < bVar1) {
      uVar3 = 0;
    }
    *(char *)(unaff_A6 + 0x9b) = (char)uVar3 + *(char *)(unaff_A6 + 0x9b);
    *(uint *)(unaff_A6 + 0x30) = (uint)uVar3 << 0x10;
    sVar2 = candidate_29f2();
    uVar3 = sVar2 - 1;
    bVar1 = (char)uVar3 + *(char *)(unaff_A6 + 0x9c);
    if ((char)bVar1 < '\0') {
      uVar3 = 1;
    }
    if (1 < bVar1) {
      uVar3 = 0;
    }
    *(char *)(unaff_A6 + 0x9c) = (char)uVar3 + *(char *)(unaff_A6 + 0x9c);
    *(uint *)(unaff_A6 + 0x34) = (uint)uVar3 << 0x10;
  }
  return;
}
