/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x019f7e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_19f7e(void)

{
  short sVar1;
  byte bVar2;
  ushort uVar3;
  byte *in_A0;
  byte *extraout_A0;
  byte *extraout_A0_00;
  byte *pbVar4;
  byte *extraout_A0_01;
  int unaff_A5;
  int unaff_A6;
  bool bVar5;

  bVar2 = *in_A0;
  if (0x11 < bVar2) goto LAB_0001a052;
  uVar3 = (ushort)bVar2 * 2;
  bVar5 = CARRY2(uVar3,uVar3);
                    /* WARNING: Switch is manually overridden */
  switch((ushort)bVar2 << 2) {
  case 0:
    if (*(short *)(in_A0 + 4) != -1) {
      candidate_c7b0c();
      in_A0 = extraout_A0;
    }
    if (*(ushort *)(unaff_A5 + 0x699e) < *(ushort *)(in_A0 + 2)) {
      return;
    }
    goto LAB_0001a046;
  case 4:
    if (*(short *)(unaff_A6 + 0x72) == 0) {
      sVar1 = *(short *)(in_A0 + 2);
      *(short *)(unaff_A6 + 0x72) = sVar1;
      if (sVar1 != 0) {
        return;
      }
    }
    else {
      sVar1 = *(short *)(unaff_A6 + 0x72) + -1;
      *(short *)(unaff_A6 + 0x72) = sVar1;
      if (sVar1 != 0) {
        return;
      }
    }
    goto LAB_0001a046;
  case 8:
    candidate_1a5ca();
    in_A0 = extraout_A0_00;
    if (!bVar5) {
      return;
    }
LAB_0001a046:
    pbVar4 = in_A0 + (short)(ushort)in_A0[1];
    while (*pbVar4 < 0x12) {
      if (BYTE_ARRAY_0001a074[(short)(ushort)*pbVar4] == 0) {
        return;
      }
      candidate_1a0a6();
      pbVar4 = extraout_A0_01;
    }
    break;
  case 0xc:
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) | 0x80;
    if (*(int *)(unaff_A6 + 0xc) != 0) {
      pbVar4 = (byte *)(*(int *)(unaff_A6 + 0xc) + 0x60);
      *pbVar4 = *pbVar4 | 0x80;
    }
    return;
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
  }
LAB_0001a052:
  __m68k_trap(0xf);
  return;
}
