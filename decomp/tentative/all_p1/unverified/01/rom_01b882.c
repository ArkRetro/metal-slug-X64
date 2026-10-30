/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01b882.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_1b882(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  ushort uVar4;
  byte bVar5;
  ushort uVar6;
  ushort uVar7;
  undefined1 uVar8;
  undefined2 unaff_D2w;
  undefined1 uVar9;
  undefined2 unaff_D3w;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  char cVar10;
  undefined8 uVar11;

  func_0x000143e4();
  uVar7 = candidate_49e8();
  if ((char)uVar7 != '\0') {
    if (uVar7 < 6) {
                    /* WARNING: Switch is manually overridden */
      switch(uVar7 << 2) {
      case 0:
        break;
      case 4:
        break;
      case 8:
        candidate_120f8();
        break;
      case 0xc:
        candidate_120f8();
        bVar3 = *(byte *)(unaff_A6 + 0x1e);
        bVar5 = bVar3 - 1;
        *(byte *)(unaff_A6 + 0x1e) = bVar5;
        if (SBORROW1(bVar3,'\x01') == (short)((ushort)bVar5 << 8) < 0) {
          uVar7 = candidate_29c6();
          *(char *)((int)unaff_A6 + 0x6e) =
               (char)((ushort)((ushort)*(byte *)((int)unaff_A6 + 0x6e) * 0x100 +
                              (uVar7 & 0x3fff) + 0x6080) >> 8);
          uVar11 = candidate_2e7c();
          *(undefined8 *)(unaff_A6 + 0xc) = uVar11;
          bVar3 = *(byte *)((int)unaff_A6 + 0x6e);
          if ((bVar3 & 0x20) != 0) {
            bVar3 = bVar3 + 0xf;
          }
          *(byte *)((int)unaff_A6 + 0x6f) = bVar3 >> 4;
          bVar3 = *(byte *)((int)unaff_A6 + 0x6e);
          if ((bVar3 & 0x20) != 0) {
            bVar3 = bVar3 + 3;
          }
          *(byte *)((int)unaff_A6 + 0x71) = bVar3 >> 2;
          candidate_411c();
          *(undefined4 *)(extraout_A0 + 0x7a) = *(undefined4 *)((int)unaff_A6 + 0x7a);
          candidate_1acf2();
          *unaff_A6 = candidate_1bbe2;
          goto LAB_0001b8a6;
        }
        break;
      case 0x10:
        break;
      case 0x14:
      }
    }
    candidate_411c();
    *(undefined4 *)(extraout_A0_00 + 0x7a) = *(undefined4 *)((int)unaff_A6 + 0x7a);
    candidate_1acf2();
    goto candidate_4188;
  }
LAB_0001b8a6:
  cVar10 = '\0';
  if (*(char *)((int)unaff_A6 + 0x79) == '\0') {
    candidate_3c5c6();
    *(char *)((int)unaff_A6 + 0x79) = -cVar10;
  }
  uVar8 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) == 0) {
    candidate_427a();
    if (0x13f < *(ushort *)(unaff_A6 + 0xf)) goto candidate_4188;
    if ((ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U) < 0x100) {
      *(char *)((int)unaff_A6 + 0x7f) = *(char *)((int)unaff_A6 + 0x7f) + '\x03';
      *(byte *)((int)unaff_A6 + 0x7f) = *(byte *)((int)unaff_A6 + 0x7f) & 0xf;
      bVar3 = *(byte *)(unaff_A6 + 0x20);
      bVar5 = bVar3 - 1;
      *(byte *)(unaff_A6 + 0x20) = bVar5;
      if (bVar5 == 0 || SBORROW1(bVar3,'\x01') != (short)((ushort)bVar5 << 8) < 0) {
        if (*(byte *)((int)unaff_A6 + 0x7e) < 5) {
          *(char *)((int)unaff_A6 + 0x7e) = *(char *)((int)unaff_A6 + 0x7e) + '\x01';
        }
        *(undefined1 *)(unaff_A6 + 0x20) = 1;
      }
      return;
    }
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (uVar7 < 0xfc11) goto candidate_4188;
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar4 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar8,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar4 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11(uVar9,-(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar7 = (ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar7 ^ uVar6);
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      candidate_505a();
    }
    if (*(char *)((int)unaff_A6 + 0x79) == '\0') {
      if (-1 < (char)(*(char *)((int)unaff_A6 + 0x6e) + -0x40)) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      }
      *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
      *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 2) == 0) {
          uVar7 = candidate_29c6();
          *(ushort *)(unaff_A6 + 10) = ((uVar7 & 0xf) - 7) + *(short *)(unaff_A6 + 10);
          if (*(char *)((int)unaff_A6 + 0x6d) != '@') {
            uVar7 = candidate_29c6();
            *(ushort *)(unaff_A6 + 0xb) = ((uVar7 & 0xf) - 7) + *(short *)(unaff_A6 + 0xb);
          }
        }
        else {
          uVar7 = candidate_29c6();
          *(ushort *)(unaff_A6 + 10) = ((uVar7 & 0xf) - 7) + *(short *)(unaff_A6 + 10);
          *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 2;
        }
      }
      else {
        uVar7 = candidate_29c6();
        *(ushort *)(unaff_A6 + 0xb) = ((uVar7 & 0xf) - 7) + *(short *)(unaff_A6 + 0xb);
      }
      candidate_4256();
      candidate_9ea8();
      *unaff_A6 = candidate_1bc82;
      return;
    }
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (uVar7 < 0xfc11) goto candidate_4188;
    uVar7 = *(ushort *)(unaff_A5 + 0x700a);
    *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
    uVar6 = uRam002fffe8;
    uVar4 = ~uVar7 & 0xfff;
    uVar7 = CONCAT11(uVar8,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                             '\x01' << (uVar4 & 7)) != 0));
    uRam002fffe0 = uVar7 & CONCAT11(uVar9,-(0x7ff < uVar4));
    uRam002fffe2 = 0;
    uRam002fffe4 = 0;
    uRam002fffe6 = 0;
    *(short *)(unaff_A5 + 0x700a) =
         ((short)-(uVar4 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
         *(short *)(unaff_A5 + 0x700a);
    uVar7 = (ushort)(byte)-(0xfbff < (ushort)(uVar4 - 0x800)) & (uVar7 ^ uVar6);
  }
  if ((uVar7 & 1) != 0) {
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
