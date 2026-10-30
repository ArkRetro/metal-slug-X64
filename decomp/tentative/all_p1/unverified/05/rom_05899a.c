/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05899a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5899a(void)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  candidate_59c2c();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0xffff8000;
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x82) = 0x8000;
  }
  candidate_120f8();
  *unaff_A6 = 0x589d2;
  iVar3 = *(int *)((int)unaff_A6 + 0x82) + *(int *)((int)unaff_A6 + 0x62);
  if (iVar3 < 0) {
    uVar1 = candidate_260c8();
    iVar2 = (uint)uVar1 * -0x100;
    if (SBORROW4(iVar3,iVar2) != (int)(iVar3 + (uint)uVar1 * 0x100) < 0) {
      iVar3 = iVar2;
    }
  }
  else {
    uVar1 = candidate_260c8();
    iVar2 = (uint)uVar1 * 0x100;
    if (iVar3 != iVar2 && SBORROW4(iVar3,iVar2) == (int)(iVar3 + (uint)uVar1 * -0x100) < 0) {
      iVar3 = iVar2;
    }
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar3;
  candidate_59214();
  if (*(short *)(unaff_A6 + 0x1e) == 2) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      if (*(short *)(unaff_A6 + 0xf) < *(short *)(unaff_A6 + 0x1d)) {
        *unaff_A6 = candidate_58c2e;
      }
    }
    else if (*(short *)((int)unaff_A6 + 0x76) < *(short *)(unaff_A6 + 0xf)) {
      *unaff_A6 = candidate_58c2e;
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0xf) < *(short *)(unaff_A6 + 0x1c)) {
      *unaff_A6 = candidate_58c2e;
    }
  }
  else if (*(short *)(unaff_A6 + 0x1c) < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_58c2e;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_58c2e;
  }
  candidate_5929a();
  return;
}
