/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d21a4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_d21a4(void)

{
  ushort uVar1;
  undefined2 uVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar2;
  candidate_d36b8();
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  *unaff_A6 = 0xd21ee;
  bVar4 = false;
  uVar3 = candidate_d3462();
  if (((bVar4) && (*(char *)((int)unaff_A6 + 0x71) != '\0')) && ((_DAT_0010000e & 7) == 0)) {
    uVar1 = *(ushort *)((int)unaff_A6 + 0x8e);
    bVar4 = CARRY2(uVar3,uVar1);
    *(ushort *)((int)unaff_A6 + 0x8e) = uVar3 + uVar1;
    candidate_d33ee();
    if (bVar4) {
      *unaff_A6 = candidate_d2222;
    }
  }
  candidate_d3220();
  candidate_d3268();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_d2266;
  }
  if (*(short *)((int)unaff_A6 + 0x6e) < 0x10) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x20;
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  bVar4 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 0x10;
  }
  candidate_d341e();
  if (bVar4) {
    *unaff_A6 = candidate_d231a;
  }
  return;
}
