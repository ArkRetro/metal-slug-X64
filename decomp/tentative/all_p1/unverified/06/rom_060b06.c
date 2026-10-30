/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x060b06.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_60b06(void)

{
  int iVar1;
  uint uVar2;
  ushort uVar3;
  undefined2 uVar4;
  int iVar5;
  uint uVar6;
  undefined4 *unaff_A6;
  undefined1 uVar7;

  candidate_1ab98();
  *(undefined2 *)(unaff_A6 + 0x1d) = *(undefined2 *)((int)unaff_A6 + 0x72);
  candidate_12022();
  *unaff_A6 = 0x60b2c;
  uVar3 = *(ushort *)((int)unaff_A6 + 0x72) - 1;
  *(ushort *)((int)unaff_A6 + 0x72) = uVar3;
  if (-1 < (int)((uint)uVar3 << 0x10)) {
    return;
  }
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x1ff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  *(undefined1 *)(unaff_A6 + 0x1f) = 0;
  candidate_1b1dc();
  candidate_4256();
  uVar4 = 0x18;
  if (*(char *)(unaff_A6[3] + 0x7d) == '\0') {
    uVar4 = 0x30;
  }
  *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar4;
  *unaff_A6 = 0x60baa;
  uVar7 = 0;
  if (*(char *)(unaff_A6[3] + 0x7d) == '\0') {
    candidate_60e7a();
    iVar1 = unaff_A6[0xc] + unaff_A6[0xf];
    iVar5 = (uint)*(ushort *)((int)unaff_A6 + 0x6e) * 0x10000;
    if (iVar1 != iVar5 &&
        SBORROW4(iVar1,iVar5) ==
        (int)(iVar1 + (uint)*(ushort *)((int)unaff_A6 + 0x6e) * -0x10000) < 0) {
      unaff_A6[0xc] = iVar5 - unaff_A6[0xf];
    }
    uVar2 = unaff_A6[0xd] + unaff_A6[0x10];
    uVar6 = (uint)*(ushort *)(unaff_A6 + 0x1c) * 0x10000;
    uVar7 = uVar2 < uVar6;
    if (SBORROW4(uVar2,uVar6) != (int)(uVar2 + (uint)*(ushort *)(unaff_A6 + 0x1c) * -0x10000) < 0) {
      unaff_A6[0xd] = uVar6 - unaff_A6[0x10];
      uVar7 = 0;
    }
  }
  else {
    candidate_60e44();
  }
  candidate_1ac38();
  candidate_427a();
  candidate_60eb0();
  if ((bool)uVar7) {
    candidate_9ea8();
    *unaff_A6 = candidate_60c2a;
  }
  return;
}
