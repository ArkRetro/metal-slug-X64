/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d9e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d9e6(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_6e2b6();
    candidate_1b1dc();
    *unaff_A6 = 0x6da04;
    uVar2 = 0;
    candidate_6e258();
    candidate_1945c();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_6df7e;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_6e27a();
      goto LAB_0006df76;
    }
  }
  candidate_6e2b6();
  *(byte *)((int)unaff_A6 + 0x82) = *(byte *)((int)unaff_A6 + 0x82) & 0xfe;
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_353a4();
  *unaff_A6 = 0x6da5c;
  uVar2 = 0;
  candidate_6e258();
  candidate_6e27a();
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6df7e;
    uVar2 = 0;
  }
  uVar1 = candidate_aa610();
  if ((bool)uVar2) {
    *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
    bVar3 = false;
    uVar1 = candidate_aa760();
    if (bVar3) {
      *(undefined2 *)(unaff_A6 + 0x1d) = uVar1;
      candidate_aa83a();
      *unaff_A6 = candidate_6dac0;
    }
  }
LAB_0006df76:
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
