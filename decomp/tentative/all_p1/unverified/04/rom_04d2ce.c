/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04d2ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_4d2ce(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  short sVar4;
  byte bVar5;
  ushort uVar6;
  ushort uVar7;
  undefined2 uVar8;
  undefined2 unaff_D2w;
  ushort uVar9;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;
  undefined1 uVar11;
  undefined8 uVar12;

  bVar10 = false;
  if (*(char *)(unaff_A6[3] + 0xa4) == '\0') {
    candidate_4dd86();
    if (bVar10) {
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x21) = uVar8;
    }
    else {
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x21) = uVar8;
    }
  }
  else {
    candidate_4dd86();
    if (bVar10) {
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x21) = uVar8;
    }
    else {
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
      uVar8 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x21) = uVar8;
    }
  }
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  *unaff_A6 = 0x4d390;
  candidate_1ac38();
  uVar11 = false;
  if (*(short *)(unaff_A6 + 0x21) != 0) {
    if (*(short *)(unaff_A6 + 0x22) == 0) {
      *(char *)(unaff_A6 + 0x18) = *(char *)(unaff_A6 + 0x18) + '\x01';
      bVar5 = *(byte *)(unaff_A6 + 0x18) & 1;
      *(byte *)(unaff_A6 + 0x18) = bVar5;
      if (bVar5 == 0) {
        bVar10 = false;
        if (*(char *)(unaff_A6[3] + 0xa4) == '\0') {
          candidate_4dd86();
          if (bVar10) {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
          }
          else {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
          }
        }
        else {
          candidate_4dd86();
          if (bVar10) {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
          }
          else {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
          }
        }
        unaff_A6[0xc] = 0;
        unaff_A6[0xd] = 0;
        uVar11 = false;
      }
      else {
        bVar10 = false;
        if (*(char *)(unaff_A6[3] + 0xa4) == '\0') {
          candidate_4dd86();
          if (bVar10) {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
            uVar8 = candidate_260c8();
            *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar8;
          }
          else {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
            uVar8 = candidate_260c8();
            *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar8;
          }
        }
        else {
          candidate_4dd86();
          if (bVar10) {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
            uVar8 = candidate_260c8();
            *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar8;
          }
          else {
            uVar8 = candidate_260c8();
            *(undefined2 *)(unaff_A6 + 0x22) = uVar8;
            uVar8 = candidate_260c8();
            *(undefined2 *)((int)unaff_A6 + 0x8e) = uVar8;
          }
        }
        candidate_1b04a();
        uVar3 = *(undefined4 *)(extraout_A0 + 0x40);
        *(short *)(extraout_A0 + 0x40) = *(short *)(extraout_A0 + 0x40) + 0x10;
        candidate_1b12e();
        *(undefined4 *)(extraout_A0 + 0x40) = uVar3;
        uVar12 = candidate_2e7c();
        *(undefined8 *)(unaff_A6 + 0xc) = uVar12;
        sVar4 = *(short *)(unaff_A6 + 0x21);
        uVar11 = sVar4 == 0;
        *(short *)(unaff_A6 + 0x21) = sVar4 + -1;
      }
    }
    else {
      *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
      uVar11 = false;
      if (((*(char *)(unaff_A6 + 0x18) != '\0') &&
          ((unaff_A6[0xc] != 0 || (uVar11 = false, unaff_A6[0xd] != 0)))) &&
         ((*(short *)(unaff_A6 + 0xf) < 0x11 ||
          (((0x11f < *(short *)(unaff_A6 + 0xf) || (*(short *)(unaff_A6 + 0x10) < 0x139)) ||
           (uVar11 = *(ushort *)(unaff_A6 + 0x10) < 0x1e8,
           0x1e7 < (short)*(ushort *)(unaff_A6 + 0x10))))))) {
        *(undefined2 *)(unaff_A6 + 0x22) = 0xffff;
        uVar11 = false;
      }
    }
  }
  candidate_427a();
  candidate_4d608();
  candidate_4e092();
  if (!(bool)uVar11) {
    return;
  }
  *(short *)(unaff_A6[3] + 0x86) = *(short *)(unaff_A6[3] + 0x86) + -1;
  uVar9 = *(ushort *)(unaff_A5 + 0x700c);
  *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
  if (0xfc10 < uVar9) {
    uVar9 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar7 = uRam002fffe8;
    uVar6 = ~uVar9 & 0xfff;
    uVar9 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                     -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar6 & 7)) !=
                      0));
    uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar6));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    if (((ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar9 ^ uVar7) & 1) != 0) {
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
