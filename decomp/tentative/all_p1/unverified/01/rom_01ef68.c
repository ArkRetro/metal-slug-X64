/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ef68.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_1ef68(void)

{
  ushort uVar1;
  int unaff_A6;
  bool bVar2;

  candidate_1b1dc();
  uVar1 = _DAT_0010000e & 3;
  if (uVar1 == 0) {
    if (*(char *)(unaff_A6 + 0x70) == '\0') {
      bVar2 = false;
      uVar1 = candidate_40bc();
      if (bVar2) {
        uVar1 = candidate_1b220();
      }
    }
    else if (*(char *)(unaff_A6 + 0x70) < '\0') {
      bVar2 = false;
      uVar1 = candidate_40bc();
      if (bVar2) {
        uVar1 = candidate_1b220();
      }
    }
    else {
      bVar2 = false;
      uVar1 = candidate_40bc();
      if (bVar2) {
        uVar1 = candidate_1b220();
      }
    }
  }
  return uVar1;
}
