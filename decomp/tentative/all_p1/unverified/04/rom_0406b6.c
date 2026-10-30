/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0406b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char candidate_406b6(void)

{
  code *pcVar1;
  byte bVar2;
  ushort uVar3;
  ushort uVar4;
  short sVar7;
  uint uVar5;
  int iVar6;
  ushort uVar8;
  char cVar9;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar10;
  undefined1 uVar11;

  if (*(char *)((int)unaff_A6 + 0x5a) == '\0') {
    return *(char *)((int)unaff_A6 + 0x5a);
  }
  candidate_4097a();
  bVar2 = *(byte *)((int)unaff_A6 + 0x5a);
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    if (0x13 < bVar2) goto LAB_00040dc0;
                    /* WARNING: Switch is manually overridden */
    switch((ushort)bVar2 * 4) {
    case 0:
      break;
    case 4:
      goto candidate_40c8e;
    case 8:
      break;
    case 0xc:
      goto LAB_00040b46;
    case 0x10:
      goto LAB_00040b46;
    case 0x14:
      break;
    case 0x18:
      cVar9 = candidate_4256();
      *unaff_A6 = 0x40df0;
      return cVar9;
    case 0x1c:
      goto LAB_00040a40;
    case 0x20:
      break;
    case 0x24:
      break;
    case 0x28:
      goto LAB_00040af6;
    case 0x2c:
      goto LAB_00040a7c;
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
    goto LAB_00040dc0;
  }
  if (0x13 < bVar2) goto LAB_00040dc0;
                    /* WARNING: Switch is manually overridden */
  switch((ushort)bVar2 * 4) {
  case 0:
    break;
  case 4:
    goto candidate_40c8e;
  case 8:
    break;
  case 0xc:
    goto LAB_00040b46;
  case 0x10:
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
  case 0x14:
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      cVar9 = candidate_4256();
    }
    else {
      cVar9 = candidate_4256();
    }
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 0x18:
    if (*(char *)((int)unaff_A6 + 0x6e) == '\0') {
      candidate_40ed0();
    }
    if (-1 < (short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                                   -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8)) {
      cVar9 = candidate_4256();
      *unaff_A6 = candidate_40db4;
      return cVar9;
    }
    cVar9 = candidate_4256();
    *unaff_A6 = candidate_40db4;
    return cVar9;
  case 0x1c:
LAB_00040a40:
    uVar5 = candidate_29c6();
    iVar6 = (uVar5 & 0xffff) * 4;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar6 = (uVar5 & 0xffff) * -4;
    }
    *(int *)((int)unaff_A6 + 0x76) = iVar6;
    unaff_A6[0xd] = 0;
    cVar9 = candidate_4256();
    *unaff_A6 = 0x40df0;
    return cVar9;
  case 0x20:
    break;
  case 0x24:
candidate_40c8e:
    uVar5 = candidate_29c6();
    iVar6 = (uVar5 & 0xffff) * 4 + -0x10000;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar6 = -iVar6;
    }
    *(int *)((int)unaff_A6 + 0x76) = iVar6;
    unaff_A6[0xc] = iVar6;
    uVar5 = candidate_29c6();
    unaff_A6[0xd] = (uVar5 & 0xffff) * 8 + 0x40000;
    bVar10 = false;
    uVar8 = candidate_29c6();
    if (bVar10) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    }
    *(ushort *)((int)unaff_A6 + 0x26) = (uVar8 & 0x3f) + 0x80;
    cVar9 = candidate_4256();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = candidate_40d06;
    return cVar9;
  case 0x28:
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
  case 0x2c:
LAB_00040a7c:
    candidate_40f04();
    sVar7 = candidate_29c6();
    if (sVar7 < 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    uVar5 = candidate_29c6();
    iVar6 = (uVar5 & 0xffff) * 4;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar6 = (uVar5 & 0xffff) * -4;
    }
    unaff_A6[0xc] = iVar6;
    unaff_A6[0xd] = 0x100000;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = candidate_40ae0;
    uVar11 = false;
    if (-1 < (int)unaff_A6[0xd]) {
      uVar5 = unaff_A6[0xd];
      uVar11 = uVar5 < (uint)((int)unaff_A6[0xd] >> 4);
      unaff_A6[0xd] = uVar5 - ((int)unaff_A6[0xd] >> 4);
    }
    candidate_405ce();
    candidate_40852();
    candidate_427a();
    if (!(bool)uVar11) {
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
      }
      else {
        if (*(char *)((int)unaff_A6 + 0x71) == '\0') {
          uVar8 = *(ushort *)(unaff_A5 + 0x700c);
          *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
          if (0xfc10 < uVar8) {
            uVar8 = *(ushort *)(unaff_A5 + 0x700a);
            *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
            uVar4 = uRam002fffe8;
            uVar3 = ~uVar8 & 0xfff;
            uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                             -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar3 & 7)) != 0));
            uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
            uRam002fffe2 = 0;
            uRam002fffe4 = 0;
            uRam002fffe6 = 0;
            *(short *)(unaff_A5 + 0x700a) =
                 ((short)-(uVar3 == 0x400) &
                 _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
                 *(short *)(unaff_A5 + 0x700a);
            if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar8 ^ uVar4) & 1) != 0) {
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
        cVar9 = *(char *)((int)unaff_A6 + 0x71) + -1;
        *(char *)((int)unaff_A6 + 0x71) = cVar9;
      }
      return cVar9;
    }
candidate_4188:
    candidate_4126();
    iVar6 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar6 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  case 0x30:
    goto LAB_00040dd8;
  case 0x34:
LAB_00040dd8:
    cVar9 = candidate_4256();
    *unaff_A6 = 0x40df0;
    return cVar9;
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
LAB_00040dc0:
  cVar9 = candidate_4256();
  *unaff_A6 = 0x40df0;
  return cVar9;
}
