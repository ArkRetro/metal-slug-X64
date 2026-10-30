/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b4b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b4b8(void)

{
  undefined1 uVar2;
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined2 *)(unaff_A6 + 0x21) = 6;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x8e) = uVar2;
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 3;
  candidate_4256();
  *unaff_A6 = 0x2b526;
  uVar1 = candidate_2be98();
  uVar2 = uVar1 < 9;
  if (uVar1 != 9) {
    candidate_2bf28();
    if ((bool)uVar2) {
      *(short *)(unaff_A6 + 0x21) = *(short *)(unaff_A6 + 0x21) + -1;
      uVar2 = 0;
      if (*(short *)(unaff_A6 + 0x21) < 1) {
        *(undefined2 *)(unaff_A6 + 0x21) = 0;
        uVar2 = *(ushort *)(unaff_A6 + 0xf) < 0x10;
        if (0xf < (short)*(ushort *)(unaff_A6 + 0xf)) {
          *unaff_A6 = candidate_2b566;
          uVar2 = 0;
        }
      }
    }
    candidate_427a();
    candidate_409a();
    if (!(bool)uVar2) {
      *unaff_A6 = candidate_2b138;
    }
    return;
  }
  candidate_2be98();
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  *unaff_A6 = candidate_2b138;
  return;
}
