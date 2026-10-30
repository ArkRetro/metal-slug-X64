/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07bc10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7bc10(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar1;

  if ((*(byte *)((int)unaff_A6 + 0x81) & 1) == 0) {
    candidate_7c0ea();
    if ((bool)in_CF) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  else {
    candidate_7c0ea();
    if ((bool)in_CF) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  *unaff_A6 = 0x7bc7c;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_7c106();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7bc10;
  }
  uVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x20;
  if (((0x1f < (short)*(ushort *)(unaff_A6 + 0xf)) &&
      (uVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x120, (short)*(ushort *)(unaff_A6 + 0xf) < 0x121)) &&
     (candidate_7c090(), (bool)uVar1)) {
    *unaff_A6 = candidate_7bcbc;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_7be22;
      uVar1 = 0;
    }
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7be86;
  }
  return;
}
