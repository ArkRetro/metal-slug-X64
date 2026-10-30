/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074dee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_74dee(void)

{
  ushort uVar1;
  int iVar2;
  undefined4 *unaff_A6;

  uVar1 = *(ushort *)(unaff_A6 + 0x21) - 1;
  *(ushort *)(unaff_A6 + 0x21) = uVar1;
  if ((int)((uint)uVar1 << 0x10) < 0) {
    candidate_1b1dc();
    *(uint *)((int)unaff_A6 + 0x7a) = (uint)*(ushort *)((int)unaff_A6 + 0x86) << 0x10;
    *unaff_A6 = 0x74e1e;
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x72) - unaff_A6[0xc] >> 3) + unaff_A6[0xc];
    *(short *)((int)unaff_A6 + 0x26) =
         (short)((int)unaff_A6[0xc] / (int)(short)((uint)*(undefined4 *)((int)unaff_A6 + 0x72) >> 8)
                ) + 0x100;
    candidate_1abc8();
    iVar2 = unaff_A6[0xf];
    candidate_1ac38();
    candidate_427a();
    iVar2 = iVar2 - unaff_A6[0xf];
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    iVar2 = *(int *)((int)unaff_A6 + 0x7a) - iVar2;
    *(int *)((int)unaff_A6 + 0x7a) = iVar2;
    if (iVar2 < 0) {
      *unaff_A6 = candidate_74e74;
    }
  }
  else {
    candidate_1abc8();
    candidate_427a();
  }
  iVar2 = *(int *)((int)unaff_A6 + 0x6e);
  if (iVar2 != 0) {
    iVar2 = candidate_140c0();
  }
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x7fff;
  if (*(short *)(unaff_A6 + 0x10) < 0xc0) {
    *unaff_A6 = candidate_41ac;
  }
  if (0x240 < *(short *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_41ac;
  }
  return iVar2;
}
