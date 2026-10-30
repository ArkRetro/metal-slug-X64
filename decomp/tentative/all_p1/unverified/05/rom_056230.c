/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x056230.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char candidate_56230(void)

{
  code *pcVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar6;
  uint uVar4;
  int iVar5;
  ushort uVar7;
  char cVar9;
  undefined2 uVar8;
  byte bVar10;
  undefined1 uVar11;
  undefined2 unaff_D2w;
  undefined1 uVar12;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar13;
  undefined1 uVar14;

  do {
    while( true ) {
      candidate_4256();
      *unaff_A6 = 0x56246;
      bVar13 = false;
      candidate_55cde();
      uVar11 = (undefined1)((ushort)unaff_D2w >> 8);
      uVar12 = (undefined1)((ushort)unaff_D3w >> 8);
      if (bVar13) break;
LAB_0005611c:
      *(undefined1 *)((int)unaff_A6 + 0x95) = 0;
      *(undefined1 *)(unaff_A6 + 0x25) = 0;
      candidate_4256();
      *unaff_A6 = candidate_5613a;
      candidate_55bae();
      uVar14 = false;
      if ((int)unaff_A6[0x21] < 0) {
        candidate_4c24();
        if ((bool)uVar14) goto LAB_00056a28;
        candidate_1436a();
        candidate_427a();
        goto LAB_00055ad6;
      }
      candidate_55d22();
      if (!(bool)uVar14) {
LAB_00055f64:
        candidate_4256();
        *unaff_A6 = candidate_55f7a;
        uVar14 = false;
        candidate_4c24();
        if ((bool)uVar14) goto LAB_00056a28;
        candidate_1436a();
        candidate_427a();
        if ((bool)uVar14) {
          *unaff_A6 = candidate_56230;
          uVar14 = 0;
        }
        goto LAB_00055ad6;
      }
    }
    bVar13 = false;
    if ((*(char *)((int)unaff_A6 + 0x95) != '\0') || (candidate_55e28(), !bVar13)) {
      bVar10 = candidate_55d4e();
      if ((bVar10 == 1) || ((uVar14 = bVar10 < 2, bVar10 == 2 && (candidate_55db2(), (bool)uVar14)))
         ) goto LAB_00055f64;
      candidate_55df6();
      if ((bool)uVar14) {
        *(undefined1 *)((int)unaff_A6 + 0x95) = 0;
        *(undefined1 *)(unaff_A6 + 0x25) = 0;
        uVar14 = false;
        candidate_55d22();
        if (!(bool)uVar14) goto LAB_00055f64;
      }
      candidate_4c24();
      if ((bool)uVar14) goto LAB_00056a28;
      candidate_55f08();
      if ((bool)uVar14) goto LAB_00055f9e;
      candidate_1456a();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        *unaff_A6 = candidate_5616c;
      }
      bVar10 = candidate_55ec8();
      if (bVar10 == 1) {
        candidate_55e9a();
        candidate_4256();
        *unaff_A6 = candidate_55f40;
        uVar14 = false;
        candidate_4c24();
        if ((bool)uVar14) goto LAB_00056a28;
        candidate_1abc8();
        candidate_427a();
        if ((bool)uVar14) {
          *unaff_A6 = candidate_56230;
          uVar14 = 0;
        }
      }
      else {
        uVar14 = bVar10 < 2;
        if (bVar10 == 2) {
          *(undefined1 *)((int)unaff_A6 + 0x95) = 0;
          *(undefined1 *)(unaff_A6 + 0x25) = 0;
          candidate_4256();
          *unaff_A6 = candidate_561d0;
          uVar14 = false;
          candidate_4c24();
          if ((bool)uVar14) goto LAB_00056a28;
          candidate_1436a();
          candidate_427a();
          if ((bool)uVar14) {
            *unaff_A6 = candidate_561f4;
            uVar14 = 0;
          }
        }
        else {
          candidate_427a();
        }
      }
      goto LAB_00055ad6;
    }
    candidate_55d22();
    if (!bVar13) goto LAB_00055f64;
    uVar8 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x8a) = uVar8;
    candidate_4256();
    *unaff_A6 = 0x56040;
    bVar13 = false;
    candidate_55cde();
    if (!bVar13) goto LAB_0005611c;
    candidate_55e64();
    if (!bVar13) {
      candidate_4256();
      *unaff_A6 = candidate_55fee;
      uVar14 = false;
      candidate_4c24();
      if ((bool)uVar14) goto LAB_00056a28;
      candidate_1436a();
      candidate_427a();
      if ((bool)uVar14) {
        *unaff_A6 = candidate_56012;
        uVar14 = 0;
      }
      goto LAB_00055ad6;
    }
    bVar10 = candidate_55d4e();
    if (bVar10 == 1) goto LAB_00055f64;
    uVar14 = bVar10 < 2;
  } while (bVar10 == 2);
  candidate_55e6e();
  if ((bool)uVar14) {
    candidate_55c92();
    *unaff_A6 = candidate_560f8;
    uVar14 = false;
    candidate_4c24();
    if ((bool)uVar14) goto LAB_00056a28;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar14) {
      *unaff_A6 = candidate_5608c;
      uVar14 = 0;
    }
LAB_00055ad6:
    cVar9 = candidate_1945c();
    if (!(bool)uVar14) {
      return cVar9;
    }
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11(uVar11,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                                '\x01' << (uVar2 & 7)) != 0));
      uRam002fffe0 = uVar7 & CONCAT11(uVar12,-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar7 ^ uVar3) & 1) != 0) {
LAB_00003c64:
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
    iVar5 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar5 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_55f08();
  if (!(bool)uVar14) {
    candidate_4c24();
    if ((bool)uVar14) goto LAB_00056a28;
    candidate_1436a();
    candidate_427a();
    goto LAB_00055ad6;
  }
LAB_00055f9e:
  candidate_4256();
  *unaff_A6 = candidate_55fb4;
  uVar14 = false;
  candidate_4c24();
  if (!(bool)uVar14) {
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar14) {
      *unaff_A6 = candidate_56012;
      uVar14 = 0;
    }
    goto LAB_00055ad6;
  }
LAB_00056a28:
  candidate_1436a();
  candidate_427a();
  bVar13 = false;
  candidate_40bc();
  if (bVar13) {
    candidate_1b220();
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0;
  if (*(char *)((int)unaff_A6 + 0x5a) == '\0') {
    return *(char *)((int)unaff_A6 + 0x5a);
  }
  candidate_4097a();
  uVar14 = *(undefined1 *)((int)unaff_A6 + 0x5a);
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    switch(uVar14) {
    case 0:
      break;
    case 1:
      goto candidate_40c8e;
    case 2:
      break;
    case 3:
      goto LAB_00040b46;
    case 4:
      goto LAB_00040b46;
    case 5:
      break;
    case 6:
      cVar9 = candidate_4256();
      *unaff_A6 = 0x40df0;
      return cVar9;
    case 7:
      goto LAB_00040a40;
    case 8:
      break;
    case 9:
      break;
    case 10:
      goto LAB_00040af6;
    case 0xb:
      goto LAB_00040a7c;
    case 0xc:
      break;
    case 0xd:
      break;
    case 0xe:
      break;
    case 0xf:
      break;
    case 0x10:
      break;
    case 0x11:
      break;
    case 0x12:
      break;
    case 0x13:
    }
    goto LAB_00040dc0;
  }
  switch(uVar14) {
  case 0:
    break;
  case 1:
    goto candidate_40c8e;
  case 2:
    break;
  case 3:
    goto LAB_00040b46;
  case 4:
LAB_00040b46:
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      if (*(char *)((int)unaff_A6 + 0x72) == '\0') {
        cVar9 = candidate_4256();
      }
      else {
        cVar9 = candidate_4256();
      }
    }
    else {
      cVar9 = candidate_4256();
    }
    unaff_A6[0xd] = 0x20000;
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 5:
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      cVar9 = candidate_4256();
    }
    else {
      cVar9 = candidate_4256();
    }
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 6:
    if (*(char *)((int)unaff_A6 + 0x6e) == '\0') {
      candidate_40ed0();
    }
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      cVar9 = candidate_4256();
      *unaff_A6 = candidate_40db4;
      return cVar9;
    }
    cVar9 = candidate_4256();
    *unaff_A6 = candidate_40db4;
    return cVar9;
  case 7:
LAB_00040a40:
    uVar4 = candidate_29c6();
    iVar5 = (uVar4 & 0xffff) * 4;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar5 = (uVar4 & 0xffff) * -4;
    }
    *(int *)((int)unaff_A6 + 0x76) = iVar5;
    unaff_A6[0xd] = 0;
    cVar9 = candidate_4256();
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 8:
    break;
  case 9:
candidate_40c8e:
    uVar4 = candidate_29c6();
    iVar5 = (uVar4 & 0xffff) * 4 + -0x10000;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar5 = -iVar5;
    }
    *(int *)((int)unaff_A6 + 0x76) = iVar5;
    unaff_A6[0xc] = iVar5;
    uVar4 = candidate_29c6();
    unaff_A6[0xd] = (uVar4 & 0xffff) * 8 + 0x40000;
    bVar13 = false;
    uVar7 = candidate_29c6();
    if (bVar13) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    }
    *(ushort *)((int)unaff_A6 + 0x26) = (uVar7 & 0x3f) + 0x80;
    cVar9 = candidate_4256();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = candidate_40d06;
    return cVar9;
  case 10:
LAB_00040af6:
    candidate_40f04();
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      cVar9 = candidate_4256();
    }
    else {
      cVar9 = candidate_4256();
    }
    unaff_A6[0xd] = 0x20000;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 0xb:
LAB_00040a7c:
    candidate_40f04();
    sVar6 = candidate_29c6();
    if (sVar6 < 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    uVar4 = candidate_29c6();
    iVar5 = (uVar4 & 0xffff) * 4;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar5 = (uVar4 & 0xffff) * -4;
    }
    unaff_A6[0xc] = iVar5;
    unaff_A6[0xd] = 0x100000;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = candidate_40ae0;
    uVar14 = false;
    if (-1 < (int)unaff_A6[0xd]) {
      uVar4 = unaff_A6[0xd];
      uVar14 = uVar4 < (uint)((int)unaff_A6[0xd] >> 4);
      unaff_A6[0xd] = uVar4 - ((int)unaff_A6[0xd] >> 4);
    }
    candidate_405ce();
    candidate_40852();
    candidate_427a();
    if (!(bool)uVar14) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
        cVar9 = *(char *)((int)unaff_A6 + 0x71);
        if (cVar9 != '\0') {
          cVar9 = cVar9 + -1;
          if (cVar9 == '\0') {
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
            cVar9 = '\x1e';
          }
          *(char *)((int)unaff_A6 + 0x71) = cVar9;
        }
        return cVar9;
      }
      if (*(char *)((int)unaff_A6 + 0x71) != '\0') {
        cVar9 = *(char *)((int)unaff_A6 + 0x71) + -1;
        *(char *)((int)unaff_A6 + 0x71) = cVar9;
        return cVar9;
      }
      uVar7 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
      if (uVar7 < 0xfc11) goto candidate_4188;
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar3 = uRam002fffe8;
      uVar2 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11(uVar11,-((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) &
                                '\x01' << (uVar2 & 7)) != 0));
      uRam002fffe0 = uVar7 & CONCAT11(uVar12,-(0x7ff < uVar2));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar2 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar2 - 0x800)) & (uVar7 ^ uVar3) & 1) == 0)
      goto candidate_4188;
      goto LAB_00003c64;
    }
    goto candidate_4188;
  case 0xc:
    goto LAB_00040dd8;
  case 0xd:
LAB_00040dd8:
    cVar9 = candidate_4256();
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 0xe:
    break;
  case 0xf:
    break;
  case 0x10:
    break;
  case 0x11:
    break;
  case 0x12:
    break;
  case 0x13:
  }
LAB_00040dc0:
  cVar9 = candidate_4256();
  *unaff_A6 = 0x40df0;
  return cVar9;
}
