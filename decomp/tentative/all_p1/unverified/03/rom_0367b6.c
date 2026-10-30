/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0367b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_367b6(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  ushort uVar8;
  undefined4 uVar7;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar9;
  undefined1 uVar10;

  if ((ushort)(*(short *)(unaff_A6 + 0xf) - 0x28U) < 0xf0) {
    *(undefined1 *)(unaff_A6 + 0x1e) = 0xff;
  }
  bVar9 = false;
  if (*(short *)(unaff_A6 + 0x1d) != 0) {
    sVar3 = *(short *)(unaff_A6 + 0x1d);
    bVar9 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x1d) = sVar3 + -1;
  }
  candidate_35af0();
  if (bVar9) {
LAB_000368da:
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    candidate_366da();
    candidate_4256();
    candidate_19dfc();
    candidate_120f8();
    *unaff_A6 = 0x3690a;
    uVar10 = 0;
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar10) {
      uVar6 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar6 + 0x3ef;
      if (0xfc10 < uVar6) {
        uVar6 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar8 = ~uVar6 & 0xfff;
        uVar6 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar6 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar8 & 7)
                           ) != 0));
        uRam002fffe0 = uVar6 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar8));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar8 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar8 - 0x800)) & (uVar6 ^ uVar4) & 1) != 0) {
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
      goto candidate_4188;
    }
  }
  else {
    candidate_1af9a();
    if (!bVar9) {
      uVar6 = *(short *)(unaff_A6 + 0xf) - *(short *)(extraout_A0 + 0x3c);
      uVar5 = (uint)uVar6;
      if ((int)unaff_A6[0xc] < 0) {
        if ((short)uVar6 < -0x2f) {
          unaff_A6[0xc] = unaff_A6[0x1c];
        }
      }
      else {
        if (0x2f < (short)uVar6) {
          uVar5 = -unaff_A6[0x1c];
          unaff_A6[0xc] = uVar5;
        }
        uVar6 = (ushort)uVar5;
      }
      if (*(short *)(unaff_A6 + 0x1d) == 0) {
        if ((short)uVar6 < 0) {
          uVar6 = -uVar6;
        }
        if (uVar6 < 5) {
          if ((*(char *)(unaff_A6 + 0x1e) != '\0') && (*(char *)((int)unaff_A6 + 0x77) != '\0')) {
            *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
            candidate_4256();
            candidate_19dfc();
            candidate_366da();
            *unaff_A6 = 0x368c2;
            uVar10 = false;
            candidate_35af0();
            if ((bool)uVar10) goto LAB_000368da;
            candidate_1ac38();
            candidate_427a();
            goto SUB_0003623e;
          }
          uVar6 = candidate_260c8();
          *(ushort *)(unaff_A6 + 0x1d) = uVar6 >> 1;
          *(undefined1 *)((int)unaff_A6 + 0x77) = 0xff;
        }
      }
    }
    unaff_A6[0xd] = 0;
    uVar6 = (*(short *)(unaff_A6 + 0x10) + -0x100) - *(short *)((int)unaff_A6 + 0x6e);
    uVar8 = uVar6;
    if ((short)uVar6 < 0) {
      uVar8 = -uVar6;
    }
    uVar10 = uVar8 < 4;
    if (4 < uVar8) {
      uVar7 = 0x18000;
      if (-1 < (short)uVar6) {
        uVar7 = 0xfffe8000;
      }
      unaff_A6[0xd] = uVar7;
      uVar10 = false;
    }
    candidate_1ac38();
    candidate_427a();
  }
SUB_0003623e:
  candidate_1945c();
  if (!(bool)uVar10) {
    return;
  }
  candidate_4192();
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
