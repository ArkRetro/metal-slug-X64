/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01351a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_1351a(void)

{
  uint in_D0;
  uint uVar1;
  undefined4 in_D1;
  uint unaff_D3;
  ushort uVar2;
  uint unaff_D6;
  ushort uVar3;
  uint unaff_D7;
  short sVar4;
  int unaff_A5;
  bool bVar5;
  undefined8 uVar6;

  uVar6 = CONCAT44(in_D0,in_D1);
  if (unaff_D3 == 0) {
    return in_D0;
  }
  sVar4 = (short)unaff_D7;
  uVar3 = (ushort)(unaff_D7 >> 0x10);
  if (sVar4 != 0) {
    uVar6 = candidate_12cba();
    uVar1 = (uint)((ulonglong)uVar6 >> 0x20);
    if ((unaff_D6 != 0x80000000) && (-1 < (int)(unaff_D3 ^ unaff_D6))) {
      bVar5 = unaff_D6 == 0;
      if ((int)unaff_D6 < 0) {
        unaff_D6 = unaff_D6 + 1;
        bVar5 = unaff_D6 == 0;
      }
      if (bVar5) {
        uVar1 = candidate_129ec();
        if (sVar4 != 0) {
          uVar1 = candidate_13204();
        }
        return uVar1;
      }
      if (-1 < (int)(unaff_D3 - unaff_D6 ^ unaff_D3)) {
        uVar6 = candidate_129ec();
        if (sVar4 != 0) {
          uVar6 = candidate_13204();
        }
        uVar1 = (uint)((ulonglong)uVar6 >> 0x20);
        if (7 < (ushort)(unaff_D6 + ((uint)uVar6 & 0x7ffff) >> 0x10)) {
          uVar1 = candidate_12938();
          if (sVar4 == 0x38) {
            *(undefined1 *)(unaff_A5 + 0x4014) = 1;
            unaff_D7 = (uint)uVar3 << 0x10;
          }
          else {
            *(undefined1 *)(unaff_A5 + 0x4014) = 0;
          }
          *(uint *)(unaff_A5 + 0x4016) = unaff_D7;
          uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
        }
        return uVar1;
      }
      if (7 < (ushort)(unaff_D6 + (unaff_D3 - unaff_D6) + ((uint)uVar6 & 0x7ffff) >> 0x10)) {
        uVar1 = candidate_12938();
        if (sVar4 == 0x38) {
          *(undefined1 *)(unaff_A5 + 0x4014) = 1;
          unaff_D7 = (uint)uVar3 << 0x10;
        }
        else {
          *(undefined1 *)(unaff_A5 + 0x4014) = 0;
        }
        *(uint *)(unaff_A5 + 0x4016) = unaff_D7;
        uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
      }
      return uVar1;
    }
  }
  uVar1 = (uint)((ulonglong)uVar6 >> 0x20);
  if ((int)unaff_D3 < 0x80001) {
    if ((int)unaff_D3 < -0x80000) {
      unaff_D3 = 0xfff80000;
    }
  }
  else {
    unaff_D3 = 0x80000;
  }
  *(short *)(unaff_A5 + 0x401a) = sVar4;
  if (*(short *)(unaff_A5 + 0x4018) == 0x3c) {
    uVar2 = (ushort)(unaff_D3 + ((uint)uVar6 & 0x7ffff) >> 0x10);
    if (7 < uVar2) {
      uVar1 = candidate_12938();
      if (sVar4 == 0x38) {
        *(undefined1 *)(unaff_A5 + 0x4014) = 1;
        unaff_D7 = (uint)uVar3 << 0x10;
      }
      else {
        *(undefined1 *)(unaff_A5 + 0x4014) = 0;
      }
      *(uint *)(unaff_A5 + 0x4016) = unaff_D7;
      uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
      uVar2 = 0xffff;
    }
    if ((short)unaff_D7 == 0x3c) {
      return uVar1;
    }
  }
  else {
    uVar2 = (ushort)(unaff_D3 + ((uint)uVar6 & 0x7ffff) >> 0x10);
    if (7 < uVar2) {
      uVar1 = candidate_12938();
      if (sVar4 == 0x38) {
        *(undefined1 *)(unaff_A5 + 0x4014) = 1;
        unaff_D7 = (uint)uVar3 << 0x10;
      }
      else {
        *(undefined1 *)(unaff_A5 + 0x4014) = 0;
      }
      *(uint *)(unaff_A5 + 0x4016) = unaff_D7;
      uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
      uVar2 = 0xffff;
    }
  }
  if ((((short)uVar2 < 0) && (sVar4 = (short)unaff_D7, sVar4 != 0)) &&
     (uVar1 = candidate_12cba(), unaff_D6 != 0x80000000)) {
    if (-1 < (int)unaff_D6) {
      unaff_D6 = unaff_D6 + 1;
    }
    if ((int)(unaff_D6 ^ unaff_D3) < 0) {
      uVar1 = candidate_129ec();
      if (sVar4 != 0) {
        uVar6 = candidate_13204();
        uVar1 = (uint)((ulonglong)uVar6 >> 0x20);
        if (7 < (ushort)(unaff_D6 + ((uint)uVar6 & 0x7ffff) >> 0x10)) {
          uVar1 = candidate_12938();
          if (sVar4 == 0x38) {
            *(undefined1 *)(unaff_A5 + 0x4014) = 1;
            unaff_D7 = unaff_D7 & 0xffff0000;
          }
          else {
            *(undefined1 *)(unaff_A5 + 0x4014) = 0;
          }
          *(uint *)(unaff_A5 + 0x4016) = unaff_D7;
          uVar1 = uVar1 << 0x10 | uVar1 >> 0x10;
        }
        return uVar1;
      }
      *(ushort *)(unaff_A5 + 0x4012) = *(ushort *)(unaff_A5 + 0x4012) | 0x100;
    }
    return uVar1;
  }
  return uVar1;
}
