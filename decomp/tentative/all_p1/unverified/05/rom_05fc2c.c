/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05fc2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5fc2c(void)

{
  ushort uVar1;
  short sVar2;
  short sVar3;
  int extraout_A0;
  int unaff_A6;
  bool bVar4;

  uVar1 = *(ushort *)(unaff_A6 + 0x74);
  if ((uVar1 == 0) || (uVar1 == 1)) {
    return;
  }
  if (uVar1 == 2) {
    if (*(char *)(unaff_A6 + 0x88) != *(char *)(unaff_A6 + 0x8c)) {
      return;
    }
  }
  else {
    bVar4 = uVar1 < 3;
    if ((uVar1 == 3) && (candidate_1af9a(), !bVar4)) {
      if ((short)(*(short *)(extraout_A0 + 0x40) - *(short *)(unaff_A6 + 0x40)) < 0x40) {
        sVar2 = *(short *)(extraout_A0 + 0x3c);
        if (sVar2 < 0) {
          sVar2 = -sVar2;
        }
        sVar3 = sVar2 + -0x7e;
        if (sVar3 < 0) {
          sVar3 = -sVar3;
        }
        sVar2 = sVar2 + -0x70;
        if (sVar2 < 0) {
          sVar2 = -sVar2;
        }
        if (sVar3 < sVar2) {
          if (*(char *)(unaff_A6 + 0x88) != '\0') {
            return;
          }
          return;
        }
      }
      if (*(char *)(unaff_A6 + 0x88) != '\0') {
        return;
      }
    }
  }
  return;
}
