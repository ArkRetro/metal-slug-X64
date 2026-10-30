/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058bc2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_58bc2(void)

{
  int iVar1;
  ushort uVar2;
  int iVar3;
  undefined4 *unaff_A6;

  iVar1 = *(int *)((int)unaff_A6 + 0x82) + *(int *)((int)unaff_A6 + 0x62);
  if (iVar1 < 0) {
    uVar2 = candidate_260c8();
    iVar3 = (uint)uVar2 * -0x80;
    if (SBORROW4(iVar1,iVar3) != (int)(iVar1 + (uint)uVar2 * 0x80) < 0) {
      *(int *)((int)unaff_A6 + 0x62) = iVar3;
      *unaff_A6 = candidate_58c2e;
    }
  }
  else {
    uVar2 = candidate_260c8();
    iVar3 = (uint)uVar2 * 0x80;
    if (iVar1 != iVar3 && SBORROW4(iVar1,iVar3) == (int)(iVar1 + (uint)uVar2 * -0x80) < 0) {
      *(int *)((int)unaff_A6 + 0x62) = iVar3;
      *unaff_A6 = candidate_58c2e;
    }
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  candidate_59214();
  candidate_5929a();
  return;
}
