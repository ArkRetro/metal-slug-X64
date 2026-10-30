/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05e388.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_5e388(void)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar4;
  int unaff_A6;
  bool bVar5;

  if (*(int *)(unaff_A6 + 0x6e) == 0) {
    return 0;
  }
  sVar1 = candidate_1b12e();
  uVar3 = (ushort)(sVar1 + 8U) >> 4 & 0xf;
  if (uVar3 == 0) {
    uVar2 = 8;
  }
  else {
    if (uVar3 < 4) {
      if (*(char *)(unaff_A6 + 0x88) == '\x10') {
        return uVar3 - 4;
      }
      cVar4 = '\x01';
      goto LAB_0005e43c;
    }
    uVar2 = uVar3 - 8;
    if ((ushort)(uVar3 - 4) < 4) {
      if (*(char *)(unaff_A6 + 0x88) == '\0') {
        return uVar2;
      }
      cVar4 = -1;
      goto LAB_0005e43c;
    }
  }
  uVar2 = uVar2 * 2;
  if ((char)uVar2 == *(char *)(unaff_A6 + 0x88)) {
    bVar5 = false;
    if (0 < *(short *)(unaff_A6 + 0x7c)) {
      return uVar2;
    }
    uVar3 = candidate_5e2e4();
    if (!bVar5) {
      return uVar3;
    }
    if (0x1e0 < *(short *)(unaff_A6 + 0x40)) {
      return uVar3;
    }
    if (*(short *)(unaff_A6 + 0x40) < 0x100) {
      return uVar3;
    }
    return uVar3;
  }
  if ((char)uVar2 < *(char *)(unaff_A6 + 0x88)) {
    if (*(char *)(unaff_A6 + 0x88) == '\0') {
      return uVar2;
    }
    cVar4 = -1;
  }
  else {
    if (*(char *)(unaff_A6 + 0x88) == '\x10') {
      return uVar2;
    }
    cVar4 = '\x01';
  }
LAB_0005e43c:
  uVar3 = _DAT_0010000e & 0xff03;
  if ((_DAT_0010000e & 3) == 0) {
    *(char *)(unaff_A6 + 0x88) = cVar4 + *(char *)(unaff_A6 + 0x88);
  }
  return uVar3;
}
