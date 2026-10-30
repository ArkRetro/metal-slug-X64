/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06ee34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6ee34(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_4256();
    candidate_1b1dc();
    *unaff_A6 = 0x6ee68;
    uVar2 = 0;
    candidate_6fcca();
    candidate_1945c();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_4188;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_6fc20();
      goto LAB_0006f674;
    }
  }
  candidate_4256();
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_353a4();
  *unaff_A6 = 0x6eec2;
  uVar2 = 0;
  candidate_6fcca();
  candidate_6fc20();
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
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
      *unaff_A6 = candidate_6ef2a;
    }
  }
LAB_0006f674:
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
