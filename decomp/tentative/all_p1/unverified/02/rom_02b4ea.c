/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b4ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b4ea(void)

{
  undefined2 uVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(char *)((int)unaff_A6 + 0x8d) = *(char *)((int)unaff_A6 + 0x8d) + '\x01';
  *(char *)((int)unaff_A6 + 0x8e) = *(char *)((int)unaff_A6 + 0x8e) + -1;
  if (*(char *)((int)unaff_A6 + 0x8e) < '\x01') {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x21) = uVar1;
    uVar3 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8e) = uVar3;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x21) = uVar1;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 3;
  candidate_4256();
  *unaff_A6 = 0x2b526;
  uVar2 = candidate_2be98();
  uVar3 = uVar2 < 9;
  if (uVar2 != 9) {
    candidate_2bf28();
    if ((bool)uVar3) {
      *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
      uVar3 = 0;
      if (*(short *)(unaff_A6 + 0x21) < 1) {
        *(undefined2 *)(unaff_A6 + 0x21) = 0;
        uVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x10;
        if (0xf < (short)*(ushort *)(unaff_A6 + 0xf)) {
          *unaff_A6 = candidate_2b566;
          uVar3 = 0;
        }
      }
    }
    candidate_427a();
    candidate_409a();
    if (!(bool)uVar3) {
      *unaff_A6 = candidate_2b138;
    }
    return;
  }
  candidate_2be98();
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  *unaff_A6 = candidate_2b138;
  return;
}
