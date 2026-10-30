/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0407ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char candidate_407ca(void)

{
  code *pcVar1;
  byte bVar2;
  ushort uVar3;
  ushort uVar4;
  char cVar8;
  short sVar7;
  uint uVar5;
  int iVar6;
  undefined4 extraout_D1;
  undefined2 unaff_D2w;
  ushort uVar9;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar10;

  if (*(char *)((int)unaff_A6 + 0x5a) == '\0') {
    return *(char *)((int)unaff_A6 + 0x5a);
  }
  candidate_4097a();
  bVar2 = *(byte *)((int)unaff_A6 + 0x5a);
  *(undefined2 *)((int)unaff_A6 + 0x5a) = 0;
  if (0x13 < bVar2) goto LAB_00040a28;
  sVar7 = (ushort)bVar2 * 4;
                    /* WARNING: Switch is manually overridden */
  switch(sVar7) {
  case 0:
    break;
  case 4:
    break;
  case 8:
    break;
  case 0xc:
    goto LAB_00040b46;
  case 0x10:
LAB_00040b46:
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      if (*(char *)((int)unaff_A6 + 0x72) == '\0') {
        cVar8 = candidate_4256();
      }
      else {
        cVar8 = candidate_4256();
      }
    }
    else {
      cVar8 = candidate_4256();
    }
    unaff_A6[0xd] = 0x20000;
    *unaff_A6 = 0x40df0;
    return cVar8;
  case 0x14:
    if ((short)(CONCAT11((char)((ushort)sVar7 >> 8),*(undefined1 *)((int)unaff_A6 + 0x5b)) ^
               (ushort)CONCAT31((int3)((uint)extraout_D1 >> 8),
                                -((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0))) < 0) {
      cVar8 = candidate_4256();
    }
    else {
      cVar8 = candidate_4256();
    }
    *unaff_A6 = 0x40df0;
    return cVar8;
  case 0x18:
    break;
  case 0x1c:
    uVar5 = candidate_29c6();
    iVar6 = (uVar5 & 0xffff) * 4;
    if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
      iVar6 = (uVar5 & 0xffff) * -4;
    }
    *(int *)((int)unaff_A6 + 0x76) = iVar6;
    unaff_A6[0xd] = 0;
    cVar8 = candidate_4256();
    *unaff_A6 = 0x40df0;
    return cVar8;
  case 0x20:
    break;
  case 0x24:
    break;
  case 0x28:
    candidate_40f04();
    if ((short)((ushort)(byte)(*(byte *)((int)unaff_A6 + 0x5b) ^
                              -((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) << 8) < 0) {
      cVar8 = candidate_4256();
    }
    else {
      cVar8 = candidate_4256();
    }
    unaff_A6[0xd] = 0x20000;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = 0x40df0;
    return cVar8;
  case 0x2c:
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
    uVar10 = false;
    if (-1 < (int)unaff_A6[0xd]) {
      uVar5 = unaff_A6[0xd];
      uVar10 = uVar5 < (uint)((int)unaff_A6[0xd] >> 4);
      unaff_A6[0xd] = uVar5 - ((int)unaff_A6[0xd] >> 4);
    }
    candidate_405ce();
    candidate_40852();
    candidate_427a();
    if (!(bool)uVar10) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
        cVar8 = *(char *)((int)unaff_A6 + 0x71);
        if (cVar8 != '\0') {
          cVar8 = cVar8 + -1;
          if (cVar8 == '\0') {
            *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
            cVar8 = '\x1e';
          }
          *(char *)((int)unaff_A6 + 0x71) = cVar8;
        }
      }
      else {
        if (*(char *)((int)unaff_A6 + 0x71) == '\0') {
          uVar9 = *(ushort *)(unaff_A5 + 0x700c);
          *(ushort *)(unaff_A5 + 0x700c) = uVar9 + 0x3ef;
          if (0xfc10 < uVar9) {
            uVar9 = *(ushort *)(unaff_A5 + 0x700a);
            *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
            uVar4 = uRam002fffe8;
            uVar3 = ~uVar9 & 0xfff;
            uVar9 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                             -((*(byte *)((short)(uVar9 >> 3 & 0xfff) + 0xdedd2) &
                               '\x01' << (uVar3 & 7)) != 0));
            uRam002fffe0 = uVar9 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
            uRam002fffe2 = 0;
            uRam002fffe4 = 0;
            uRam002fffe6 = 0;
            *(short *)(unaff_A5 + 0x700a) =
                 ((short)-(uVar3 == 0x400) &
                 _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
                 *(short *)(unaff_A5 + 0x700a);
            if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar9 ^ uVar4) & 1) != 0) {
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
        cVar8 = *(char *)((int)unaff_A6 + 0x71) + -1;
        *(char *)((int)unaff_A6 + 0x71) = cVar8;
      }
      return cVar8;
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
LAB_00040a28:
  cVar8 = candidate_4256();
  *unaff_A6 = 0x40df0;
  return cVar8;
}
