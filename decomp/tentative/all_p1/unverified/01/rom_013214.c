/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x013214.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_13214(void)

{
  int iVar1;
  uint in_D0;
  uint uVar2;
  uint uVar3;
  uint unaff_D2;
  ushort uVar4;
  uint unaff_D6;
  int unaff_D7;
  ushort uVar5;
  ushort uVar6;
  int unaff_A5;
  bool bVar7;

  if (unaff_D2 == 0) {
    return in_D0;
  }
  uVar5 = (ushort)unaff_D7;
  uVar6 = (ushort)((uint)unaff_D7 >> 0x10);
  if (((uVar5 != 0) && (in_D0 = candidate_12ad4(), unaff_D6 != 0x80000000)) &&
     (-1 < (int)(unaff_D2 ^ unaff_D6))) {
    bVar7 = unaff_D6 == 0;
    if ((int)unaff_D6 < 0) {
      unaff_D6 = unaff_D6 + 1;
      bVar7 = unaff_D6 == 0;
    }
    if (bVar7) {
      uVar2 = candidate_129ec();
      if (uVar5 != 0) {
        uVar2 = candidate_12ee2();
      }
      return uVar2;
    }
    if (-1 < (int)(unaff_D2 - unaff_D6 ^ unaff_D2)) {
      uVar2 = candidate_129ec();
      if (uVar5 != 0) {
        uVar2 = candidate_12ee2();
      }
      uVar3 = unaff_D6 + uVar2;
      if (7 < (ushort)(unaff_D6 + (uVar2 & 0x7ffff) >> 0x10)) {
        uVar2 = candidate_12938();
        if (uVar5 == 0x38) {
          *(undefined1 *)(unaff_A5 + 0x4014) = 1;
          unaff_D7 = (uint)uVar6 << 0x10;
        }
        else if (*(char *)(unaff_A5 + 0x4014) == '\x01') {
          *(undefined1 *)(unaff_A5 + 0x4014) = 2;
          if (*(uint *)(unaff_A5 + 0x4004) != 0) {
            uVar5 = uVar5 >> 2;
            if ((*(uint *)(unaff_A5 + 0x4004) & 1 << (uVar5 & 0x1f)) != 0) {
              uVar5 = 0;
              *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x20;
            }
            unaff_D7 = CONCAT22(uVar6,uVar5 << 2);
          }
        }
        *(int *)(unaff_A5 + 0x4016) = unaff_D7;
        uVar3 = uVar2 << 0x10 | uVar2 >> 0x10;
      }
      return uVar3;
    }
    iVar1 = unaff_D6 + (unaff_D2 - unaff_D6);
    uVar2 = iVar1 + in_D0;
    if (7 < (ushort)(iVar1 + (in_D0 & 0x7ffff) >> 0x10)) {
      uVar2 = candidate_12938();
      if (uVar5 == 0x38) {
        *(undefined1 *)(unaff_A5 + 0x4014) = 1;
        unaff_D7 = (uint)uVar6 << 0x10;
      }
      else if (*(char *)(unaff_A5 + 0x4014) == '\x01') {
        *(undefined1 *)(unaff_A5 + 0x4014) = 2;
        if (*(uint *)(unaff_A5 + 0x4004) != 0) {
          uVar5 = uVar5 >> 2;
          if ((*(uint *)(unaff_A5 + 0x4004) & 1 << (uVar5 & 0x1f)) != 0) {
            uVar5 = 0;
            *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x20;
          }
          unaff_D7 = CONCAT22(uVar6,uVar5 << 2);
        }
      }
      *(int *)(unaff_A5 + 0x4016) = unaff_D7;
      uVar2 = uVar2 << 0x10 | uVar2 >> 0x10;
    }
    return uVar2;
  }
  if ((int)unaff_D2 < 0x80001) {
    if ((int)unaff_D2 < -0x80000) {
      unaff_D2 = 0xfff80000;
    }
  }
  else {
    unaff_D2 = 0x80000;
  }
  *(ushort *)(unaff_A5 + 0x401a) = uVar5;
  if (*(short *)(unaff_A5 + 0x4018) == 0x3c) {
    uVar2 = unaff_D2 + in_D0;
    uVar4 = (ushort)(unaff_D2 + (in_D0 & 0x7ffff) >> 0x10);
    if (7 < uVar4) {
      uVar2 = candidate_12938();
      if (uVar5 == 0x38) {
        *(undefined1 *)(unaff_A5 + 0x4014) = 1;
        unaff_D7 = (uint)uVar6 << 0x10;
      }
      else if (*(char *)(unaff_A5 + 0x4014) == '\x01') {
        *(undefined1 *)(unaff_A5 + 0x4014) = 2;
        if (*(uint *)(unaff_A5 + 0x4004) != 0) {
          uVar5 = uVar5 >> 2;
          if ((*(uint *)(unaff_A5 + 0x4004) & 1 << (uVar5 & 0x1f)) != 0) {
            uVar5 = 0;
            *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x20;
          }
          unaff_D7 = CONCAT22(uVar6,uVar5 << 2);
        }
      }
      *(int *)(unaff_A5 + 0x4016) = unaff_D7;
      uVar2 = uVar2 << 0x10 | uVar2 >> 0x10;
      uVar4 = 0xffff;
    }
    if ((short)unaff_D7 == 0x3c) {
      return uVar2;
    }
  }
  else {
    uVar2 = unaff_D2 + in_D0;
    uVar4 = (ushort)(unaff_D2 + (in_D0 & 0x7ffff) >> 0x10);
    if (7 < uVar4) {
      uVar2 = candidate_12938();
      if (uVar5 == 0x38) {
        *(undefined1 *)(unaff_A5 + 0x4014) = 1;
        unaff_D7 = (uint)uVar6 << 0x10;
      }
      else if (*(char *)(unaff_A5 + 0x4014) == '\x01') {
        *(undefined1 *)(unaff_A5 + 0x4014) = 2;
        if (*(uint *)(unaff_A5 + 0x4004) != 0) {
          uVar5 = uVar5 >> 2;
          if ((*(uint *)(unaff_A5 + 0x4004) & 1 << (uVar5 & 0x1f)) != 0) {
            uVar5 = 0;
            *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x20;
          }
          unaff_D7 = CONCAT22(uVar6,uVar5 << 2);
        }
      }
      *(int *)(unaff_A5 + 0x4016) = unaff_D7;
      uVar2 = uVar2 << 0x10 | uVar2 >> 0x10;
      uVar4 = 0xffff;
    }
  }
  if ((((short)uVar4 < 0) && (uVar6 = (ushort)unaff_D7, uVar6 != 0)) &&
     (uVar2 = candidate_12ad4(), unaff_D6 != 0x80000000)) {
    if (-1 < (int)unaff_D6) {
      unaff_D6 = unaff_D6 + 1;
    }
    if ((int)(unaff_D6 ^ unaff_D2) < 0) {
      uVar2 = candidate_129ec();
      if (uVar6 != 0) {
        uVar3 = candidate_12ee2();
        uVar2 = unaff_D6 + uVar3;
        if (7 < (ushort)(unaff_D6 + (uVar3 & 0x7ffff) >> 0x10)) {
          uVar2 = candidate_12938();
          uVar5 = (ushort)((uint)unaff_D7 >> 0x10);
          if (uVar6 == 0x38) {
            *(undefined1 *)(unaff_A5 + 0x4014) = 1;
            unaff_D7 = (uint)uVar5 << 0x10;
          }
          else if (*(char *)(unaff_A5 + 0x4014) == '\x01') {
            *(undefined1 *)(unaff_A5 + 0x4014) = 2;
            if (*(uint *)(unaff_A5 + 0x4004) != 0) {
              uVar6 = uVar6 >> 2;
              if ((*(uint *)(unaff_A5 + 0x4004) & 1 << (uVar6 & 0x1f)) != 0) {
                uVar6 = 0;
                *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x20;
              }
              unaff_D7 = CONCAT22(uVar5,uVar6 << 2);
            }
          }
          *(int *)(unaff_A5 + 0x4016) = unaff_D7;
          uVar2 = uVar2 << 0x10 | uVar2 >> 0x10;
        }
        return uVar2;
      }
      *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x100;
    }
    return uVar2;
  }
  return uVar2;
}
