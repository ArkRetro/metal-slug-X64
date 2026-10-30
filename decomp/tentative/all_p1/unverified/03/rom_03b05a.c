/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03b05a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_3b05a(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  short sVar5;
  ushort uVar6;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x72) = 1;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0x27;
  *(undefined1 *)((int)unaff_A6 + 0x76) = 0;
  *unaff_A6 = 0x3b070;
  uVar6 = *(ushort *)(unaff_A5 + 0x699e);
  if (*(ushort *)((int)unaff_A6 + 0x6e) <= uVar6) {
    func_0x0003cf12();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  bVar3 = *(byte *)((int)unaff_A6 + 0x76);
  *(byte *)((int)unaff_A6 + 0x76) = bVar3 + 0x80;
  if (0x7f < bVar3) {
    return uVar6;
  }
  uVar4 = *(short *)(unaff_A6 + 0x1c) + 4U & 0xc;
  *(ushort *)(unaff_A6 + 0x1c) = uVar4;
  uVar6 = *(ushort *)((int)unaff_A6 + 0x72);
  do {
    sVar5 = uVar6 * 0x20;
    _DAT_003c0000 = sVar5 + 0x701b;
    sRam003c0002 = uVar4 + (uVar6 & 3) + -0x52d0;
    uVar6 = uVar6 + 1;
  } while ((short)uVar6 < *(short *)(unaff_A6 + 0x1d));
  return sVar5 + 0x701c;
}
