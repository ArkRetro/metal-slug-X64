/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06e480.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6e480(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_6ecbc();
    candidate_4256();
    candidate_1b1dc();
    *unaff_A6 = 0x6e4aa;
    uVar2 = 0;
    candidate_6ec92();
    candidate_1945c();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_6e95c;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_6ecb4();
      goto LAB_0006e884;
    }
  }
  candidate_6ecbc();
  candidate_4256();
  candidate_1b1dc();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  candidate_353a4();
  *unaff_A6 = 0x6e508;
  uVar2 = 0;
  candidate_6ec92();
  candidate_6ecb4();
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6e95c;
    uVar2 = 0;
  }
  uVar1 = candidate_aa610();
  if ((bool)uVar2) {
    *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar1;
    bVar3 = false;
    uVar1 = candidate_aa760();
    if (bVar3) {
      *(undefined2 *)(unaff_A6 + 0x1c) = uVar1;
      candidate_aa83a();
      *unaff_A6 = candidate_6e56c;
    }
  }
LAB_0006e884:
  candidate_13f1e();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(byte *)(unaff_A6 + 0x20) = *(byte *)(unaff_A6 + 0x20) & 0xfe;
  return;
}
