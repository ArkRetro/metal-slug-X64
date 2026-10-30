/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x012ad4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_12ad4(void)

{
  uint in_D1;
  int unaff_D2;
  undefined2 unaff_D7w;
  int unaff_A5;

                    /* WARNING: Switch is manually overridden */
  switch(unaff_D7w) {
  case 0:
    break;
  case 4:
    goto LAB_00012b50;
  case 8:
    goto LAB_00012b6e;
  case 0xc:
    goto LAB_00012b92;
  case 0x10:
    goto LAB_00012bb6;
  case 0x14:
    return;
  case 0x18:
    goto LAB_00012bec;
  case 0x1c:
    goto LAB_00012c10;
  case 0x20:
    goto LAB_00012c34;
  case 0x24:
    goto candidate_12c58;
  case 0x28:
    return;
  case 0x2c:
    break;
  case 0x30:
    break;
  case 0x34:
    break;
  case 0x38:
    break;
  case 0x3c:
    if (unaff_D2 < 0) goto LAB_00012ca8;
    goto LAB_00012c96;
  case 0x40:
    break;
  case 0x44:
LAB_00012b50:
    if ((in_D1 & 0x7ffff) < 0x20000) {
      return;
    }
    break;
  case 0x48:
LAB_00012b6e:
    if ((in_D1 & 0x7ffff) - 0x20000 < 0x20000) {
      return;
    }
    break;
  case 0x4c:
LAB_00012b92:
    if ((in_D1 & 0x7ffff) - 0x40000 < 0x20000) {
      return;
    }
    break;
  case 0x50:
LAB_00012bb6:
    if ((in_D1 & 0x7ffff) - 0x60000 < 0x20000) {
      return;
    }
    break;
  case 0x54:
    return;
  case 0x58:
LAB_00012bec:
    if ((in_D1 & 0x7ffff) - 0x60000 < 0x20000) {
      return;
    }
    break;
  case 0x5c:
LAB_00012c10:
    if ((in_D1 & 0x7ffff) - 0x40000 < 0x20000) {
      return;
    }
    break;
  case 0x60:
LAB_00012c34:
    if ((in_D1 & 0x7ffff) - 0x20000 < 0x20000) {
      return;
    }
    break;
  case 100:
candidate_12c58:
    if ((in_D1 & 0x7ffff) < 0x20000) {
      return;
    }
    break;
  case 0x68:
    return;
  case 0x6c:
LAB_00012ca8:
    if (*(char *)(unaff_A5 + 0x400a) == '\0') {
      return;
    }
    break;
  case 0x70:
    break;
  case 0x74:
LAB_00012c96:
    if (*(char *)(unaff_A5 + 0x400a) == '\0') {
      return;
    }
  }
  return;
}
