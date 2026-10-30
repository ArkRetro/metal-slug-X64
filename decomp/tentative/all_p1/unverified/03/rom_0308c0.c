/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0308c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_308c0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  undefined2 extraout_D0u;
  undefined2 extraout_D0u_00;
  uint uVar7;
  undefined2 uVar8;
  byte bVar9;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;

  candidate_1228c();
  uVar7 = candidate_9c0a();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0xf;
  *unaff_A6 = 0x308e4;
  sVar3 = *(short *)((int)unaff_A6 + 0x7e);
  *(short *)((int)unaff_A6 + 0x7e) = sVar3 + -1;
  if (sVar3 != 0) {
    return uVar7;
  }
  *(bool *)(unaff_A6 + 0x24) = *(char *)(unaff_A5 + -0x8000) != '\0';
  if (*(char *)(unaff_A6 + 0x24) == '\0') {
    uVar8 = 0xf;
  }
  else {
    uVar8 = 10;
  }
  *(undefined2 *)(unaff_A6 + 10) = uVar8;
  *(undefined2 *)(unaff_A6 + 0xb) = 6;
  candidate_30162();
  bVar10 = false;
  candidate_1aeb0();
  if (bVar10) {
    candidate_40bc();
    *(undefined2 *)(extraout_A0 + 0x80) = 0;
    *(undefined1 *)(extraout_A0 + 0x90) = *(undefined1 *)(unaff_A6 + 0x24);
    *(ushort *)(unaff_A6 + 0x20) = *(ushort *)(unaff_A6 + 0x20) | 1;
  }
  else {
    _DAT_0010c5fc = 0;
    bVar10 = false;
    candidate_1aeb0();
    if (!bVar10) {
      _DAT_0010c600 = 0;
      goto LAB_00030a34;
    }
  }
  bVar10 = false;
  candidate_1aeb0();
  if (bVar10) {
    candidate_40bc();
    *(undefined2 *)(extraout_A0_00 + 0x80) = 1;
    *(undefined1 *)(extraout_A0_00 + 0x90) = *(undefined1 *)(unaff_A6 + 0x24);
    *(ushort *)(unaff_A6 + 0x20) = *(ushort *)(unaff_A6 + 0x20) | 2;
    uVar8 = extraout_D0u_00;
  }
  else {
    _DAT_0010c600 = 0;
    uVar8 = extraout_D0u;
  }
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  *unaff_A6 = 0x309cc;
  uVar7 = CONCAT22(uVar8,*(short *)((int)unaff_A6 + 0x82));
  if (*(short *)((int)unaff_A6 + 0x82) == *(short *)(unaff_A6 + 0x20)) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = 1;
  }
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
      if (((*(ushort *)(unaff_A6 + 0x20) & 1) != 0) && ((DAT_0010c35d & 0x30) != 0)) {
        *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      }
      uVar7 = CONCAT22(uVar8,*(ushort *)(unaff_A6 + 0x20)) & 0xffff0002;
      if (((*(ushort *)(unaff_A6 + 0x20) & 2) != 0) &&
         (uVar7 = CONCAT22((short)(uVar7 >> 0x10),(ushort)DAT_0010c363) & 0xffff0030,
         (DAT_0010c363 & 0x30) != 0)) {
        *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
      }
    }
    return uVar7;
  }
LAB_00030a34:
  candidate_30284();
  candidate_9c0a();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0xf;
  *unaff_A6 = 0x30a5e;
  uVar7 = candidate_427a();
  sVar3 = *(short *)((int)unaff_A6 + 0x7e);
  *(short *)((int)unaff_A6 + 0x7e) = sVar3 + -1;
  if (sVar3 != 0) {
    return uVar7;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 1;
  uVar4 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar4 + 0x3ef;
  if (0xfc10 < uVar4) {
    uVar4 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar5 = ~uVar4 & 0xfff;
    bVar9 = -((*(byte *)((short)(uVar4 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar5 & 7)) != 0);
    uRam002fffe0 = (ushort)(bVar9 & -(0x7ff < uVar5));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar5 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar5 - 0x800)) & (bVar9 ^ uVar6) & 1) != 0) {
      *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
      _DAT_002ffff0 = 1;
      do {
      } while (DAT_002fffff != '\x01');
      uRam00400000 = 0;
      uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
      uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
      uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
      uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
      uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
      uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
      uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
      uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
      uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
      uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
      uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
      uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
      uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
      uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
      uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
      _DAT_00401ffe = 0;
      func_0x0003cf12();
      candidate_3e2a8();
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
