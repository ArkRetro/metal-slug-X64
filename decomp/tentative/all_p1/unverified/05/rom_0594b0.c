/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0594b0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_594b0(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 uVar6;
  short sVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int iVar9;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;
  bool bVar11;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x81) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x17;
  candidate_120f8();
  candidate_1abc8();
  if ((*(byte *)(unaff_A6[3] + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x82) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
  *(undefined1 *)(unaff_A6 + 0x21) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x85) = 0;
  *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(unaff_A6[3] + 0x40);
  *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + 0x18;
  uVar6 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar6;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
    unaff_A6[0x14] = 0x58702;
  }
  else {
    unaff_A6[0x14] = 0x5870e;
  }
  sVar7 = candidate_260c8();
  *(int *)((int)unaff_A6 + 0x6e) = (int)sVar7 << 8;
  candidate_4c1c();
  *unaff_A6 = 0x595b2;
  func_0x000143e4();
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) && (*(char *)(unaff_A6[3] + 0x8f) == '\0')) {
    candidate_427a();
    if (*(char *)((int)unaff_A6 + 0x86) != '\0') {
      bVar3 = *(byte *)((int)unaff_A6 + 0x85);
      *(byte *)((int)unaff_A6 + 0x85) = bVar3 + 0xa0;
      if (bVar3 < 0x60) {
        candidate_599f0();
      }
      *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x01';
    }
    if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) &&
       ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)) {
      if (*(char *)((int)unaff_A6 + 0x82) != '\0') {
        uVar10 = *(ushort *)(unaff_A6 + 0x1f) < *(ushort *)(unaff_A6 + 0x10);
        if ((short)*(ushort *)(unaff_A6 + 0x10) < (short)*(ushort *)(unaff_A6 + 0x1f)) {
          candidate_597de();
        }
        candidate_1affc();
        if ((!(bool)uVar10) &&
           (*(short *)(unaff_A6 + 0x10) < (short)(*(short *)(extraout_A0 + 0x40) + 0x14))) {
          candidate_597de();
        }
      }
      iVar2 = *(int *)((int)unaff_A6 + 0x6e);
      iVar9 = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xc];
      if (iVar9 < 0) {
        if (SBORROW4(iVar9,-iVar2) != iVar9 + iVar2 < 0) {
          iVar9 = -iVar2;
        }
      }
      else if (iVar2 < iVar9) {
        iVar9 = iVar2;
      }
      unaff_A6[0xc] = iVar9;
      unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
      if (*(short *)((int)unaff_A6 + 0x7e) == 0) {
        if (*(char *)((int)unaff_A6 + 0x81) == '\0') {
          sVar7 = candidate_49e8();
        }
        else {
          sVar7 = candidate_49e8();
        }
        bVar11 = false;
        if ((sVar7 != 0) && (bVar11 = sVar7 == 0, sVar7 != 1)) goto LAB_000596de;
      }
      else {
        sVar7 = *(short *)((int)unaff_A6 + 0x7e);
        bVar11 = sVar7 == 0;
        *(short *)((int)unaff_A6 + 0x7e) = sVar7 + -1;
      }
      candidate_1945c();
      if (!bVar11) {
        return;
      }
      goto candidate_4188;
    }
  }
LAB_000596de:
  candidate_5980c();
  uVar8 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
  if (0xfc10 < uVar8) {
    uVar8 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar5 = uRam002fffe8;
    uVar4 = ~uVar8 & 0xfff;
    uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar4 & 7)) !=
                      0));
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
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
