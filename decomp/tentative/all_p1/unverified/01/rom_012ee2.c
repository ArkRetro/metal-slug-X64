/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x012ee2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_12ee2(void)

{
  uint uVar1;
  int iVar2;
  int unaff_D2;
  int unaff_D3;
  byte bVar3;
  ushort uVar4;
  uint uVar5;
  ushort uVar6;
  ushort uVar7;
  undefined2 unaff_D7w;
  int unaff_A5;

  if (unaff_D3 == 0 && unaff_D2 == 0) {
    return;
  }
  uVar1 = *(uint *)(unaff_A5 + 0x4024);
  if ((short)uVar1 == -1) {
    return;
  }
  uVar5 = uVar1 << 0x10 | uVar1 >> 0x10;
  if ((short)(uVar1 >> 0x10) == -1) {
    return;
  }
  if (*(short *)(unaff_A5 + 0x4016) == 9) {
    uVar5 = CONCAT22((short)uVar1,0xffff);
  }
  uVar6 = (ushort)uVar5;
  uVar4 = (ushort)(uVar5 >> 0x10);
                    /* WARNING: Switch is manually overridden */
  switch(unaff_D7w) {
  case 0:
    return;
  case 4:
    break;
  case 8:
    break;
  case 0xc:
    break;
  case 0x10:
    break;
  case 0x14:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x42;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0x6000;
    goto LAB_0001304a;
  case 0x18:
    goto LAB_000130ac;
  case 0x1c:
    goto LAB_000130ac;
  case 0x20:
    goto LAB_000130ac;
  case 0x24:
LAB_000130ac:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x42;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0x3600;
LAB_000130b8:
    if (uVar5 != 0) {
      iVar2 = unaff_D2 + unaff_D3 * 4;
      if (iVar2 == 0) {
        return;
      }
      bVar3 = -(iVar2 < 0);
      uVar7 = CONCAT11((char)bVar3 >> 7,~bVar3);
      uVar6 = uVar7 & uVar6;
      if (uVar6 != 0) {
        if ((char)uVar6 != '\0') {
          unaff_D2 = unaff_D2 + 3;
        }
        uVar6 = ~uVar6;
        *(ushort *)(unaff_A5 + 0x4026) = uVar6 | *(ushort *)(unaff_A5 + 0x4026);
        unaff_D3 = -(unaff_D2 >> 2);
        unaff_D2 = (unaff_D2 >> 2) << 2;
      }
      uVar7 = uVar7 & uVar4;
      if (uVar7 != 0) {
        uVar7 = ~uVar7;
        *(ushort *)(unaff_A5 + 0x4024) = uVar7 | *(ushort *)(unaff_A5 + 0x4024);
        unaff_D2 = unaff_D3 * -4;
      }
      if (CONCAT22(uVar6,uVar7) != 0) {
        return;
      }
    }
    if (unaff_D2 + unaff_D3 * 4 == 0) {
      return;
    }
    return;
  case 0x28:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x42;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0x2000;
    goto LAB_0001314a;
  case 0x2c:
    goto candidate_1319e;
  case 0x30:
    goto candidate_1319e;
  case 0x34:
    goto LAB_000131b4;
  case 0x38:
    return;
  case 0x3c:
    if (0 < unaff_D2) goto LAB_000131d6;
    if (-1 < unaff_D2) {
      return;
    }
    goto LAB_000131ec;
  case 0x40:
candidate_1319e:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x42;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0x4000;
    *(undefined1 *)(unaff_A5 + 0x4027) = 0xff;
    return;
  case 0x44:
    goto LAB_00012f9a;
  case 0x48:
    goto LAB_00012f9a;
  case 0x4c:
    goto LAB_00012f9a;
  case 0x50:
LAB_00012f9a:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x12;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0xca00;
    if (*(char *)(unaff_A5 + 0x4027) == '\0') {
      unaff_D3 = unaff_D3 + -1;
    }
    goto LAB_00012fc0;
  case 0x54:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x12;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0xe000;
    if (*(char *)(unaff_A5 + 0x4027) == '\0') {
      unaff_D3 = unaff_D3 + -1;
    }
LAB_0001304a:
    if (uVar5 != 0) {
      if (unaff_D3 == unaff_D2) {
        return;
      }
      bVar3 = -(-1 < unaff_D3 - unaff_D2);
      uVar7 = CONCAT11((char)bVar3 >> 7,~bVar3);
      uVar6 = uVar7 & uVar6;
      if (uVar6 != 0) {
        *(ushort *)(unaff_A5 + 0x4026) = uVar6 | *(ushort *)(unaff_A5 + 0x4026);
        unaff_D3 = unaff_D2;
      }
      uVar4 = ~uVar7 & uVar4;
      if (uVar4 != 0) {
        *(ushort *)(unaff_A5 + 0x4024) = uVar4 | *(ushort *)(unaff_A5 + 0x4024);
        unaff_D2 = unaff_D3;
      }
      if (CONCAT22(uVar6,uVar4) != 0) {
        return;
      }
    }
    if (unaff_D3 == unaff_D2) {
      return;
    }
    return;
  case 0x58:
    goto LAB_00013092;
  case 0x5c:
    goto LAB_00013092;
  case 0x60:
    goto LAB_00013092;
  case 100:
LAB_00013092:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x12;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0xb600;
    if (*(char *)(unaff_A5 + 0x4027) == '\0') {
      unaff_D3 = unaff_D3 + -1;
    }
    goto LAB_000130b8;
  case 0x68:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x12;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0xa000;
    if (*(char *)(unaff_A5 + 0x4027) == '\0') {
      unaff_D3 = unaff_D3 + -1;
    }
LAB_0001314a:
    if (uVar5 != 0) {
      if (unaff_D3 + unaff_D2 == 0) {
        return;
      }
      bVar3 = -(unaff_D3 + unaff_D2 < 0);
      uVar7 = CONCAT11((char)bVar3 >> 7,~bVar3);
      uVar6 = uVar7 & uVar6;
      if (uVar6 != 0) {
        uVar6 = ~uVar6;
        *(ushort *)(unaff_A5 + 0x4026) = uVar6 | *(ushort *)(unaff_A5 + 0x4026);
        unaff_D3 = -unaff_D2;
      }
      uVar7 = uVar7 & uVar4;
      if (uVar7 != 0) {
        uVar7 = ~uVar7;
        *(ushort *)(unaff_A5 + 0x4024) = uVar7 | *(ushort *)(unaff_A5 + 0x4024);
        unaff_D2 = -unaff_D3;
      }
      if (CONCAT22(uVar6,uVar7) != 0) {
        return;
      }
    }
    if (unaff_D3 + unaff_D2 == 0) {
      return;
    }
    return;
  case 0x6c:
LAB_000131ec:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 6;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0;
    *(undefined1 *)(unaff_A5 + 0x4025) = 0xff;
    return;
  case 0x70:
LAB_000131b4:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x12;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0xc000;
    *(undefined1 *)(unaff_A5 + 0x4026) = 0xff;
    return;
  case 0x74:
LAB_000131d6:
    *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 6;
    *(undefined2 *)(unaff_A5 + 0x4068) = 0x8000;
    *(undefined1 *)(unaff_A5 + 0x4024) = 0xff;
    return;
  }
  *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x42;
  *(undefined2 *)(unaff_A5 + 0x4068) = 0x4a00;
LAB_00012fc0:
  if (uVar5 != 0) {
    if (unaff_D3 * 4 == unaff_D2) {
      return;
    }
    bVar3 = -(-1 < unaff_D3 * 4 - unaff_D2);
    uVar7 = CONCAT11((char)bVar3 >> 7,~bVar3);
    uVar6 = uVar7 & uVar6;
    if (uVar6 != 0) {
      *(ushort *)(unaff_A5 + 0x4026) = uVar6 | *(ushort *)(unaff_A5 + 0x4026);
      if ((char)uVar6 != '\0') {
        unaff_D2 = unaff_D2 + 3;
      }
      unaff_D3 = unaff_D2 >> 2;
      unaff_D2 = unaff_D3 << 2;
    }
    uVar4 = ~uVar7 & uVar4;
    if (uVar4 != 0) {
      *(ushort *)(unaff_A5 + 0x4024) = uVar4 | *(ushort *)(unaff_A5 + 0x4024);
      unaff_D2 = unaff_D3 << 2;
    }
    if (CONCAT22(uVar6,uVar4) != 0) {
      return;
    }
  }
  if (unaff_D3 << 2 == unaff_D2) {
    return;
  }
  return;
}
