/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2d14.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2d14(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  candidate_4256();
  *unaff_A6 = 0xd2d4c;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_d2d74;
    uVar3 = 0;
  }
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar1 = (short)*(int *)((int)unaff_A6 + 0x82);
  if (*(int *)((int)unaff_A6 + 0x82) != 0) {
    sVar1 = candidate_49e8();
  }
  if (sVar1 != 0) {
    *unaff_A6 = candidate_d289a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d289a;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
