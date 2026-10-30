/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03ce8e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3ce8e(void)

{
  short sVar1;
  int iVar2;
  byte bVar3;
  byte bVar4;
  ushort uVar5;
  ushort uVar6;
  int unaff_A5;

  iVar2 = _DAT_0010c376;
  uVar5 = *(ushort *)(unaff_A5 + 0x437c);
  while( true ) {
    if (0x1ff < uVar5) {
      *(undefined1 *)(unaff_A5 + 0x4382) = 2;
      return;
    }
    bVar3 = *(byte *)(iVar2 + (short)uVar5);
    bVar4 = 0xff;
    if (uVar5 == *(ushort *)(unaff_A5 + 0x437a)) {
      bVar4 = 0xf;
      bVar3 = bVar3 & 0xf;
    }
    uVar6 = uVar5;
    if (bVar3 != *(byte *)(unaff_A5 + 0x437e)) break;
    *(undefined1 *)(unaff_A5 + 0x437e) = 0;
    uVar6 = uVar5 + 1;
    if (bVar3 == bVar4) {
      if (*(char *)(iVar2 + (short)uVar6) != '\0') break;
      uVar6 = uVar5 + 2;
    }
    sVar1 = *(short *)(unaff_A5 + 0x437a);
    *(ushort *)(unaff_A5 + 0x437a) = uVar6;
    _DAT_0010c374 = _DAT_0010c374 ^ (ushort)(1 << (*(byte *)(iVar2 + sVar1) >> 4));
    uVar5 = uVar6;
  }
  *(ushort *)(unaff_A5 + 0x437c) = uVar6;
  *(char *)(unaff_A5 + 0x437e) = *(char *)(unaff_A5 + 0x437e) + '\x01';
  return;
}
