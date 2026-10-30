/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06af44.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6af44(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  short sVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xf7;
  candidate_19dfc();
  if (*(byte *)((int)unaff_A6 + 0x5a) < 0x14) {
                    /* WARNING: Switch is manually overridden */
    switch((ushort)*(byte *)((int)unaff_A6 + 0x5a) * 4) {
    case 0:
      break;
    case 4:
      break;
    case 8:
      break;
    case 0xc:
      break;
    case 0x10:
      break;
    case 0x14:
      break;
    case 0x18:
      break;
    case 0x1c:
      candidate_9f88();
      candidate_9ea8();
      candidate_29c6();
      iVar3 = candidate_2e7c();
      unaff_A6[0xc] = iVar3 + unaff_A6[0xc];
      sVar4 = candidate_29c6();
      if (sVar4 < 0) {
        candidate_4256();
        *unaff_A6 = candidate_6b15c;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          uVar1 = unaff_A6[0xc];
          uVar5 = uVar1 < (uint)((int)unaff_A6[0xc] >> 4);
          unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 4);
          candidate_143dc();
          candidate_427a();
        }
        else {
          candidate_4256();
          *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
          *unaff_A6 = 0x6b19c;
          uVar2 = *(int *)((int)unaff_A6 + 0x62) >> 3;
          uVar1 = *(uint *)((int)unaff_A6 + 0x62);
          uVar5 = uVar1 < uVar2;
          *(uint *)((int)unaff_A6 + 0x62) = uVar1 - uVar2;
          candidate_6b7e0();
          candidate_427a();
          if ((bool)uVar5) {
            *unaff_A6 = candidate_6b1be;
            uVar5 = 0;
          }
        }
      }
      else {
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          candidate_4256();
          *unaff_A6 = candidate_6b0e4;
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            uVar1 = unaff_A6[0xc];
            uVar5 = uVar1 < (uint)((int)unaff_A6[0xc] >> 4);
            unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 4);
            candidate_143dc();
            candidate_427a();
            goto LAB_0006b1c6;
          }
          candidate_4256();
        }
        else {
          candidate_4256();
        }
        *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
        *unaff_A6 = 0x6b124;
        uVar2 = *(int *)((int)unaff_A6 + 0x62) >> 3;
        uVar1 = *(uint *)((int)unaff_A6 + 0x62);
        uVar5 = uVar1 < uVar2;
        *(uint *)((int)unaff_A6 + 0x62) = uVar1 - uVar2;
        candidate_6b7e0();
        candidate_427a();
        if ((bool)uVar5) {
          *unaff_A6 = candidate_6b1be;
          uVar5 = 0;
        }
      }
      goto LAB_0006b1c6;
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
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_4256();
    *unaff_A6 = 0x6afda;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      uVar1 = unaff_A6[0xc];
      uVar5 = uVar1 < (uint)((int)unaff_A6[0xc] >> 4);
      unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 4);
      candidate_143dc();
      candidate_427a();
      goto LAB_0006b1c6;
    }
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
  *unaff_A6 = 0x6b02e;
  uVar2 = *(int *)((int)unaff_A6 + 0x62) >> 3;
  uVar1 = *(uint *)((int)unaff_A6 + 0x62);
  uVar5 = uVar1 < uVar2;
  *(uint *)((int)unaff_A6 + 0x62) = uVar1 - uVar2;
  candidate_6b7e0();
  candidate_427a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_6b050;
    uVar5 = 0;
  }
LAB_0006b1c6:
  candidate_1945c();
  if ((bool)uVar5) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
