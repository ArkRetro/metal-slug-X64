/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0289a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_289a6(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar2;
  undefined1 uVar3;

  candidate_28e42();
  if ((bool)in_CF) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  if ((*(byte *)((int)unaff_A6 + 0x7d) & 0x10) == 0) {
    bVar2 = false;
    candidate_40bc();
    if (bVar2) {
      candidate_1b220();
    }
  }
  else {
    bVar2 = false;
    candidate_40bc();
    if (bVar2) {
      candidate_1b220();
    }
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 0;
  *unaff_A6 = 0x28a32;
  candidate_28e2c();
  uVar1 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar3 = 0xfffe < uVar1;
  *(ushort *)((int)unaff_A6 + 0x1e) = uVar1 + 1;
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_28660;
    uVar3 = 0;
  }
  candidate_409a();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
