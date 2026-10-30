/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02731e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2731e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_27820();
  *unaff_A6 = 0x27330;
  candidate_1abc8();
  candidate_1ac38();
  *(byte *)(unaff_A6 + 0x21) = (byte)(*(char *)((int)unaff_A6 + 0x85) + 4U) >> 3;
  uVar2 = 0;
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_27616;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_27616;
  }
  uVar2 = false;
  candidate_19424();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_27616;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
