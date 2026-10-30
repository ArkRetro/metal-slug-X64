/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01fbac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1fbac(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8801;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8801;
  candidate_9ea8();
  unaff_A6[0x14] = 0xdfff6;
  candidate_4256();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -0x80;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = (uint)*(ushort *)((int)unaff_A6 + 0x6e) << 8;
  if ((*(byte *)(unaff_A6 + 0x23) & 2) == 0) {
    if ((*(byte *)(unaff_A6 + 0x23) & 4) == 0) {
      uVar1 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
    }
    else {
      uVar1 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
    }
  }
  else if ((*(byte *)(unaff_A6 + 0x23) & 4) == 0) {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  }
  *(undefined4 *)((int)unaff_A6 + 0x82) = 0xe002e;
  candidate_4c1c();
  candidate_40bc();
  *unaff_A6 = candidate_1fc9e;
  return;
}
