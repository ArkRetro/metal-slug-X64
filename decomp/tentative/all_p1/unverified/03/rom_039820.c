/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x039820.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_39820(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar5;
  char cVar6;
  undefined1 uVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined1 uVar9;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;
  undefined1 uVar11;

  candidate_120f8();
  uVar7 = (undefined1)((ushort)unaff_D2w >> 8);
  uVar9 = (undefined1)((ushort)unaff_D3w >> 8);
  if ((*(char *)((int)unaff_A6 + 0x5a) != '\n') &&
     ((*(char *)((int)unaff_A6 + 0x5a) == '\v' || (2 < (*(char *)((int)unaff_A6 + 0x5b) + 1U & 7))))
     ) goto LAB_00039934;
  sVar5 = candidate_29f2();
  if (sVar5 == 0) {
    if (0x13 < *(byte *)((int)unaff_A6 + 0x5a)) goto LAB_00039934;
                    /* WARNING: Switch is manually overridden */
    switch((ushort)*(byte *)((int)unaff_A6 + 0x5a) * 4) {
    case 0:
      break;
    case 4:
      break;
    case 8:
      break;
    case 0xc:
      goto LAB_0003993a;
    case 0x10:
      goto LAB_0003993a;
    case 0x14:
      break;
    case 0x18:
      break;
    case 0x1c:
      break;
    case 0x20:
      break;
    case 0x24:
      break;
    case 0x28:
      break;
    case 0x2c:
      break;
    case 0x30:
      break;
    case 0x34:
      break;
    case 0x38:
      break;
    case 0x3c:
      break;
    case 0x40:
      break;
    case 0x44:
      break;
    case 0x48:
      break;
    case 0x4c:
    }
    goto LAB_00039934;
  }
  if (0x13 < *(byte *)((int)unaff_A6 + 0x5a)) goto LAB_0003993a;
                    /* WARNING: Switch is manually overridden */
  switch((ushort)*(byte *)((int)unaff_A6 + 0x5a) * 4) {
  case 0:
    break;
  case 4:
    break;
  case 8:
    goto LAB_00039934;
  case 0xc:
    break;
  case 0x10:
    break;
  case 0x14:
    break;
  case 0x18:
    break;
  case 0x1c:
    break;
  case 0x20:
    break;
  case 0x24:
LAB_00039934:
    candidate_a9cb6();
    candidate_a9dc0();
    unaff_A6[0x14] = 0;
    bVar10 = false;
    candidate_1b2be();
    if (bVar10) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    }
    candidate_4256();
    candidate_19dfc();
    *unaff_A6 = 0xa946c;
    uVar11 = 0;
    candidate_1436a();
    candidate_427a();
    if (!(bool)uVar11) {
      candidate_a9a56();
      return;
    }
    uVar8 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
    if (0xfc10 < uVar8) {
      uVar8 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar4 = uRam002fffe8;
      uVar3 = ~uVar8 & 0xfff;
      uVar8 = CONCAT11(uVar7,-((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar3 & 7)) != 0));
      uRam002fffe0 = uVar8 & CONCAT11(uVar9,-(0x7ff < uVar3));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar4) & 1) == 0)
      goto candidate_4188;
      goto LAB_00003c64;
    }
    goto candidate_4188;
  case 0x28:
    break;
  case 0x2c:
    break;
  case 0x30:
    break;
  case 0x34:
    break;
  case 0x38:
    break;
  case 0x3c:
    break;
  case 0x40:
    break;
  case 0x44:
    break;
  case 0x48:
    break;
  case 0x4c:
  }
LAB_0003993a:
  candidate_19dfc();
  candidate_4256();
  bVar10 = false;
  candidate_40bc();
  if (bVar10) {
    candidate_1b220();
  }
  candidate_40ed0();
  if (*(char *)((int)unaff_A6 + 0x5a) == '\n') {
    unaff_A6[0xc] = 0x640000;
  }
  else {
    unaff_A6[0xc] = 0x200000;
  }
  unaff_A6[0xd] = 0x60000;
  *(undefined4 *)((int)unaff_A6 + 0x7e) = 0x20000;
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    unaff_A6[0xc] = -unaff_A6[0xc];
    *(int *)((int)unaff_A6 + 0x7e) = -*(int *)((int)unaff_A6 + 0x7e);
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
  *unaff_A6 = 0x399dc;
  candidate_39ab2();
  candidate_427a();
  candidate_39b4a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
    if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
      cVar6 = *(char *)((int)unaff_A6 + 0x8e) + -1;
      if (cVar6 == '\0') {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
        cVar6 = '\x1e';
      }
      *(char *)((int)unaff_A6 + 0x8e) = cVar6;
    }
  }
  else {
    if (*(char *)((int)unaff_A6 + 0x8e) == '\0') {
      uVar8 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
      if (0xfc10 < uVar8) {
        uVar8 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar3 = ~uVar8 & 0xfff;
        uVar8 = CONCAT11(uVar7,-((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                                 '\x01' << (uVar3 & 7)) != 0));
        uRam002fffe0 = uVar8 & CONCAT11(uVar9,-(0x7ff < uVar3));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar4) & 1) != 0) {
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
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    *(char *)((int)unaff_A6 + 0x8e) = *(char *)((int)unaff_A6 + 0x8e) + -1;
  }
  return;
}
