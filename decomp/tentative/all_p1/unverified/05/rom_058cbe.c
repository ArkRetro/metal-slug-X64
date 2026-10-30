/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058cbe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_58cbe(void)

{
  ushort uVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  undefined4 *unaff_A6;
  bool bVar5;

  *(undefined2 *)(unaff_A6 + 0x20) = 2;
  candidate_59c2c();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  sVar2 = candidate_29f2();
  *(short *)((int)unaff_A6 + 0x7a) = sVar2 + 1;
  *unaff_A6 = 0x58ce4;
  candidate_59214();
  if (*(short *)(unaff_A6 + 0x1e) != 3) {
    if (*(char *)((int)unaff_A6 + 0x8f) == '\0') {
      if (*(short *)(unaff_A6 + 0x1e) == 1) {
        if (*(short *)(unaff_A6 + 0xf) < 0x130) {
          *unaff_A6 = 0x58dc0;
        }
      }
      else {
        sVar2 = *(short *)((int)unaff_A6 + 0x86);
        *(short *)((int)unaff_A6 + 0x86) = sVar2 + -1;
        if (sVar2 == 0) {
          *unaff_A6 = 0x58dc0;
        }
      }
    }
    else if (((*(short *)(unaff_A6 + 0x1e) != 0) && (*(short *)(unaff_A6 + 0x1e) != 1)) &&
            (sVar2 = *(short *)((int)unaff_A6 + 0x7a), *(short *)((int)unaff_A6 + 0x7a) = sVar2 + -1
            , sVar2 == 0)) {
      *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
      *unaff_A6 = candidate_5899a;
    }
LAB_00058dba:
    candidate_5929a();
    return;
  }
  bVar5 = false;
  if (*(char *)((int)unaff_A6 + 0x93) == '\0') {
    sVar2 = candidate_59efc();
    if (!bVar5) {
      sVar2 = candidate_59efc();
      if (bVar5) {
        if (sVar2 == 0) goto LAB_00058b84;
        sVar2 = *(short *)((int)unaff_A6 + 0x86);
        *(short *)((int)unaff_A6 + 0x86) = sVar2 + -1;
        if (sVar2 == 0) {
          *(undefined2 *)(unaff_A6 + 0x20) = 3;
          candidate_59c2c();
          *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
          *unaff_A6 = 0x58dd4;
          candidate_59214();
          if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
            *unaff_A6 = candidate_58e30;
            *(undefined2 *)((int)unaff_A6 + 0x86) = 0x1e;
          }
          candidate_591fe();
          candidate_5929a();
          return;
        }
      }
      goto LAB_00058dba;
    }
    if (sVar2 != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x93) = 0xff;
LAB_00058b84:
      candidate_120f8();
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
      *(undefined2 *)(unaff_A6 + 0x20) = 0;
      candidate_59c2c();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        *(undefined4 *)((int)unaff_A6 + 0x82) = 0xffff8000;
      }
      else {
        *(undefined4 *)((int)unaff_A6 + 0x82) = 0x8000;
      }
      *unaff_A6 = candidate_58bc2;
      iVar4 = *(int *)((int)unaff_A6 + 0x82) + *(int *)((int)unaff_A6 + 0x62);
      if (iVar4 < 0) {
        uVar1 = candidate_260c8();
        iVar3 = (uint)uVar1 * -0x80;
        if (SBORROW4(iVar4,iVar3) != (int)(iVar4 + (uint)uVar1 * 0x80) < 0) {
          *(int *)((int)unaff_A6 + 0x62) = iVar3;
          *unaff_A6 = candidate_58c2e;
        }
      }
      else {
        uVar1 = candidate_260c8();
        iVar3 = (uint)uVar1 * 0x80;
        if (iVar4 != iVar3 && SBORROW4(iVar4,iVar3) == (int)(iVar4 + (uint)uVar1 * -0x80) < 0) {
          *(int *)((int)unaff_A6 + 0x62) = iVar3;
          *unaff_A6 = candidate_58c2e;
        }
      }
      *(int *)((int)unaff_A6 + 0x62) = iVar4;
      candidate_59214();
      candidate_5929a();
      return;
    }
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  candidate_120f8();
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  candidate_59c2c();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0xffff8000;
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x8000;
  }
  *(char *)((int)unaff_A6 + 0x92) = -(*(char *)((int)unaff_A6 + 0x92) == '\0');
  *(undefined1 *)((int)unaff_A6 + 0x93) = 0;
  *unaff_A6 = candidate_58aec;
  iVar4 = *(int *)((int)unaff_A6 + 0x82) + *(int *)((int)unaff_A6 + 0x62);
  if (iVar4 < 0) {
    uVar1 = candidate_260c8();
    iVar3 = (uint)uVar1 * -0x100;
    if (SBORROW4(iVar4,iVar3) != (int)(iVar4 + (uint)uVar1 * 0x100) < 0) {
      iVar4 = iVar3;
    }
  }
  else {
    uVar1 = candidate_260c8();
    iVar3 = (uint)uVar1 * 0x100;
    if (iVar4 != iVar3 && SBORROW4(iVar4,iVar3) == (int)(iVar4 + (uint)uVar1 * -0x100) < 0) {
      iVar4 = iVar3;
    }
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar4;
  candidate_59214();
  if (*(int *)((int)unaff_A6 + 0x62) < 0) {
    if (*(short *)(unaff_A6 + 0xf) < *(short *)(unaff_A6 + 0x1d)) {
      *unaff_A6 = candidate_58c2e;
    }
  }
  else if (*(short *)((int)unaff_A6 + 0x76) < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_58c2e;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_58c2e;
  }
  candidate_5929a();
  return;
}
