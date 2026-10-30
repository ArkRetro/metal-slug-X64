/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d57a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d57a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  candidate_19dfc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
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
        candidate_4256();
        candidate_9f88();
        candidate_9ea8();
        *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
        *unaff_A6 = candidate_6d75e;
        uVar1 = 0;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          candidate_143dc();
          candidate_427a();
        }
        else {
          candidate_4256();
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
          *unaff_A6 = 0x6d792;
          uVar1 = 0;
          candidate_6d852();
          candidate_427a();
          if ((bool)uVar1) {
            *unaff_A6 = candidate_6d7c2;
            uVar1 = 0;
          }
        }
        goto LAB_0006d7aa;
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
    candidate_40bc();
    candidate_4256();
    *unaff_A6 = candidate_6d6fc;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 6);
      *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
    }
    uVar1 = 0;
    candidate_6d852();
    candidate_427a();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_6d7c2;
      uVar1 = 0;
    }
  }
  else {
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
        candidate_4256();
        candidate_9f88();
        candidate_9ea8();
        *unaff_A6 = candidate_6d6be;
        uVar1 = 0;
        candidate_6d852();
        candidate_427a();
        if ((bool)uVar1) {
          *unaff_A6 = candidate_6d7c2;
          uVar1 = 0;
        }
        goto LAB_0006d7aa;
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
    candidate_4256();
    *unaff_A6 = 0x6d67c;
    uVar1 = 0;
    candidate_6d852();
    candidate_427a();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_6d7c2;
      uVar1 = 0;
    }
  }
LAB_0006d7aa:
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_6d7c2;
  }
  return;
}
