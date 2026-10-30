/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x037e86.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_37e86(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  short extraout_D1w;
  undefined1 uVar6;
  ushort uVar7;
  undefined2 unaff_D3w;
  int unaff_A2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;

  uVar6 = 0xff;
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    uVar6 = 0;
  }
  candidate_1ab98();
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x37eda;
  bVar8 = false;
  sVar5 = candidate_37c0a();
  if (!bVar8) {
    uVar7 = sVar5 - *(short *)(unaff_A6 + 10);
    if ((short)uVar7 < 0) {
      uVar7 = -uVar7;
    }
    if (uVar7 < 9) {
      uVar7 = extraout_D1w - *(short *)(unaff_A6 + 0xb);
      if ((short)uVar7 < 0) {
        uVar7 = -uVar7;
      }
      if (uVar7 < 8) {
        if (*(char *)(unaff_A2 + 0x5a) == '\0') {
          *(undefined1 *)(unaff_A2 + 0x5a) = 0xff;
        }
        else {
          *(byte *)(unaff_A2 + 0x5f) = *(byte *)(unaff_A2 + 0x5f) | 0x80;
        }
        uVar7 = *(ushort *)(unaff_A5 + 0x700c);
        *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
        if (0xfc10 < uVar7) {
          uVar7 = *(ushort *)(unaff_A5 + 0x700a);
          *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
          uVar4 = uRam002fffe8;
          uVar3 = ~uVar7 & 0xfff;
          uVar7 = CONCAT11(uVar6,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                                   '\x01' << (uVar3 & 7)) != 0));
          uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
          uRam002fffe2 = 0;
          uRam002fffe4 = 0;
          uRam002fffe6 = 0;
          *(short *)(unaff_A5 + 0x700a) =
               ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU)
               + *(short *)(unaff_A5 + 0x700a);
          if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4) & 1) != 0) {
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
    }
  }
  candidate_1ac38();
  candidate_427a();
  candidate_37512();
  return;
}
