/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b498.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b498(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  uVar1 = candidate_2be98();
  uVar2 = uVar1 < 9;
  if (uVar1 != 9) {
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2b4b8;
      uVar2 = 0;
    }
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
