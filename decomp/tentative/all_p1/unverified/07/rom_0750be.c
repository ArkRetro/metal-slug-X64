/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0750be.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_750be(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 uVar7;
  uint uVar6;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x9a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
  *(undefined1 *)(unaff_A6 + 0x27) = 0;
  uVar7 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x96) = uVar7;
  uVar7 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x26) = uVar7;
  uVar7 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar7;
  *unaff_A6 = 0x75106;
  uVar7 = candidate_7668a();
  *(undefined2 *)((int)unaff_A6 + 0x7e) = uVar7;
  *unaff_A6 = 0x75114;
  if (*(short *)(unaff_A6 + 0x1c) == 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar5 = uRam002fffe8;
      uVar4 = ~uVar8 & 0xfff;
      uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7))
                        != 0));
      uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar4));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar8 ^ uVar5) & 1) != 0) {
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
  *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + 1;
  if (*(short *)(unaff_A6 + 0x26) <= *(short *)(unaff_A6 + 0x21)) {
    uVar6 = 0;
    *(undefined2 *)(unaff_A6 + 0x21) = 0;
    do {
      if (((*(byte *)(unaff_A6 + 0x27) & '\x01' << (uVar6 & 7)) != 0) &&
         ((*(byte *)((int)unaff_A6 + 0x9d) & '\x01' << (uVar6 & 7)) == 0)) goto LAB_0007515c;
      uVar8 = (short)uVar6 + 1;
      uVar6 = (uint)uVar8;
    } while ((short)uVar8 < 8);
    uVar6 = 0;
    do {
      if ((*(byte *)(unaff_A6 + 0x27) & '\x01' << (uVar6 & 7)) == 0) break;
      uVar8 = (short)uVar6 + 1;
      uVar6 = (uint)uVar8;
    } while ((short)uVar8 < 8);
LAB_0007515c:
    candidate_40bc();
    *(short *)(extraout_A0 + 0x7e) = (short)uVar6;
    *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) | '\x01' << (uVar6 & 7);
    *(byte *)((int)unaff_A6 + 0x9d) = *(byte *)((int)unaff_A6 + 0x9d) | '\x01' << (uVar6 & 7);
    sVar3 = (short)uVar6 * 2 + 0x86;
    if (*(short *)((int)unaff_A6 + (int)sVar3) == -1) {
      *(undefined2 *)((int)unaff_A6 + (int)sVar3) = *(undefined2 *)((int)unaff_A6 + 0x4e);
    }
    *(undefined2 *)(extraout_A0 + 0x82) = *(undefined2 *)((int)unaff_A6 + (int)sVar3);
    *(undefined2 *)(extraout_A0 + 0x4e) = *(undefined2 *)((int)unaff_A6 + 0x4e);
    *(short *)((int)unaff_A6 + 0x9a) = *(short *)((int)unaff_A6 + 0x9a) + 1;
    if ((*(short *)(unaff_A6 + 0x1c) <= *(short *)((int)unaff_A6 + 0x9a)) ||
       (*(short *)((int)unaff_A6 + 0x72) <= *(short *)((int)unaff_A6 + 0x9a))) {
      *unaff_A6 = candidate_751be;
    }
  }
  return;
}
