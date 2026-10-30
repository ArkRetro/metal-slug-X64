/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0288d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_288d2(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *(byte *)((int)unaff_A6 + 0x7d) = *(byte *)((int)unaff_A6 + 0x7d) & 0xef;
  candidate_4256();
  *unaff_A6 = 0x28930;
  uVar2 = 0;
  candidate_28e2c();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_28948;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_289a6;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  candidate_409a();
  if (!(bool)uVar2) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
