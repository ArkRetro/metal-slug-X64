/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0570a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_570a0(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  ushort uVar6;
  undefined2 uVar7;
  byte bVar8;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;

  candidate_56fcc();
  candidate_120f8();
  candidate_40bc();
  *(undefined2 *)(unaff_A6 + 0x1d) = 1;
  uVar7 = candidate_57006();
  *(undefined2 *)(unaff_A6 + 0x1f) = uVar7;
  *(undefined2 *)(unaff_A6 + 0x1d) = 0;
  uVar7 = candidate_57006();
  *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar7;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
  *unaff_A6 = 0x570e6;
  sVar5 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar5;
  if (sVar5 != 0) {
    return;
  }
  candidate_120f8();
  candidate_40bc();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x57114;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
  *unaff_A6 = 0x5712a;
  sVar5 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar5;
  if (sVar5 != 0) {
    return;
  }
  candidate_40bc();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x5714e;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xf;
  *unaff_A6 = 0x57164;
  sVar5 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar5;
  if (sVar5 != 0) {
    return;
  }
  candidate_40bc();
  candidate_40bc();
  bVar9 = _DAT_0010c472 <= _DAT_0010c46a;
  *(ushort *)(extraout_A0_00 + 0x74) = (ushort)!bVar9;
  *(ushort *)(extraout_A0 + 0x74) = (ushort)bVar9;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x571ba;
  if (*(char *)(unaff_A6 + 0x18) != '\x02') {
    return;
  }
  candidate_9c0a();
  candidate_1228c();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x14;
  *unaff_A6 = 0x571ea;
  sVar5 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar5;
  if (sVar5 != 0) {
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
  uVar3 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar3 + 0x3ef;
  if (0xfc10 < uVar3) {
    uVar3 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar4 = ~uVar3 & 0xfff;
    bVar8 = -((*(byte *)((short)(uVar3 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7)) != 0);
    uRam002fffe0 = (ushort)(bVar8 & -(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (CONCAT11(0xff,bVar8) ^ uVar6) & 1) !=
        0) {
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
