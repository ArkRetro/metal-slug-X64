/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b61c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b61c(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  candidate_9ea8();
  candidate_4256();
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x881c;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x881c;
  *unaff_A6 = 0x2b656;
  candidate_2bef2();
  uVar1 = candidate_1ad8c();
  uVar3 = uVar1 < 9;
  if (uVar1 != 9) {
    candidate_427a();
    candidate_409a();
    if (!(bool)uVar3) {
      *unaff_A6 = candidate_2b138;
    }
    return;
  }
  candidate_2be98();
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  *unaff_A6 = candidate_2b138;
  return;
}
