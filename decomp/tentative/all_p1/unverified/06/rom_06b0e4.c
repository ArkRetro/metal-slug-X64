/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b0e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b0e4(void)

{
  uint uVar1;
  uint uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    uVar1 = unaff_A6[0xc];
    uVar3 = uVar1 < (uint)((int)unaff_A6[0xc] >> 4);
    unaff_A6[0xc] = uVar1 - ((int)unaff_A6[0xc] >> 4);
    candidate_143dc();
    candidate_427a();
  }
  else {
    candidate_4256();
    *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
    *unaff_A6 = 0x6b124;
    uVar2 = *(int *)((int)unaff_A6 + 0x62) >> 3;
    uVar1 = *(uint *)((int)unaff_A6 + 0x62);
    uVar3 = uVar1 < uVar2;
    *(uint *)((int)unaff_A6 + 0x62) = uVar1 - uVar2;
    candidate_6b7e0();
    candidate_427a();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_6b1be;
      uVar3 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = 0x6b27e;
  }
  return;
}
